<?hh // strict
  class Server {
    private ?resource $sock = null;
    public function __construct(
      string $host, int $port, string $pem, string $pempass
    ) {
      $context = stream_context_create();
      stream_context_set_option($context, "ssl", "local_cert", $pem);
      stream_context_set_option($context, "ssl", "passphrase", $pempass);
      stream_context_set_option($context, "ssl", "allow_self_signed", true);
      stream_context_set_option($context, "ssl", "verify_peer", false);

      $errmsg = null;
      $errno = null;
      $tmpsock = stream_socket_server(
        "tcp://$host:$port",
        $errno,
        $errmsg,
        STREAM_SERVER_BIND|STREAM_SERVER_LISTEN,
        $context
      );
      if($tmpsock === false){
        Log::f("Sock", "Not able to create socket");
        throw new RuntimeException();
      }
      $this->sock = $tmpsock;
      stream_socket_enable_crypto($this->sock, false);
      stream_set_blocking($this->sock, 1);
    }
    public async function accept(): Awaitable<resource> {
      return socket_accept($this->sock);
    }
    public function __destruct(): void {
      fclose($this->sock);
    }
  }
