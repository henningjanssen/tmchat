<?php
  require_once 'Server.class.php';
  
  class Service{
    public static $run = true;

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

      while(self::$run){
        $new = false;
        do{
          $new = $server->accept();
        } while($new !== false && Server::$run);
        if(self::$run){
          usleep(1200000); //1.2s
        }
      }
    }
  }
