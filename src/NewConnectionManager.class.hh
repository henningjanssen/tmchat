<?hh // strict
  require_once 'ClientContainer.class.hh';

  class NewConnectionManager {
    public function __construct(
      private ClientContainer $container, private int $timeout
    ){}

    public function handle(resource $sock): void {
      throw new Exception("unimplemented");

      $read = "";
      while(true){
        $r = socket_read($sock, 512);
        if($r === false){
          break;
        }
        $read .= $r;
      }
      //TODO: login-stuff
      $id = 0; // TODO: replace with real user-id
      $client = new Client($id, $this->timeout);
      $client->addSock($sock);
      $this->container->add($client);
    }
  }
