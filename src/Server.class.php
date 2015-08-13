<?php
  class Server{
    private $sock = null;
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
  }
