<?php
  class Server{
    private $sock = null;
    public static $run = true;
    public function init($host, $port, $pem, $pempass){
      $context = stream_context_create();
      stream_context_set_option($context, "ssl", "local_cert", $pem);
      stream_context_set_option($context, "ssl", "passphrase", $pempass);
      stream_context_set_option($context, "ssl", "allow_self_signed", true);
      stream_context_set_option($context, "ssl", "verify_peer", false);

      $this->sock = stream_socket_server(
        "tcp://$host:$port",
        $errno,
        $errmsg,
        STREAM_SERVER_BIND|STREAM_SERVER_LISTEN,
        $context
      );
      //TODO: handle errors
      stream_socket_enable_crypto($this->sock, false);
      stream_set_blocking($this->sock, 0);
    }
    public function accept(){
      $new = socket_accept($this->sock);
      return $new;
    }
    public function __destruct(){
      fclose($this->close());
    }

    public static function run(){
      //load configuration
      $conff = __DIR__."/conf/conf"
        .(file_exists(__DIR__."/dev")?"-dev":"")
        .".json";
      if(!file_exists($conff)){
        echo "fatal: config-file not found: $conff";
      }
      $jconf = file_get_contents($conff);
      $conf = json_decode($jconf, true);
      unset($jconf);

      //set process title
      cli_set_process_title($jconf["processtitle"]);

      //open port and accept users
      $server = new Server();
      $server->init(
        $conf["connection"]["port"],
        $conf["connection"]["pem_file"],
        $conf["connection"]["pem_pass"]
      );

      while(Server::$run){
        $new = false;
        do{
          $new = $server->accept();
        } while($new !== false && Server::$run);
        if(Server::$run){
          usleep(1200000); //1.2s
        }
      }
    }
  }
