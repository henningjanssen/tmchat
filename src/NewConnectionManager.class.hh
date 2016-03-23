<?hh // partial
  require_once 'ClientContainer.class.hh';
  require_once __DIR__.'/exceptions/ClientRejectedException.class.hh';

  class NewConnectionManager {
    public function __construct(
      private ClientContainer $container, private int $timeout
    ){}

    public function handle(resource $sock): void {
      $read = "";
      while(true){
        $r = socket_read($sock, 512);
        if($r === false){
          break;
        }
        $read .= $r;
      }
      $id = -1;
      $protocol = "";
      $answer = "";
      try{
        list($protocol, $answer, $id) = $this->login($read);
      }
      catch(InvalidMessageException $crex){ // t-rex smaller brother
        $answer = "505 ".$crex->getMessage()."\0";
      }
      socket_write($sock, $answer);
      $client = new Client($id, $this->timeout);
      $client->addSock($sock, $protocol);
      $this->container->add($client);
    }

    private function login(string $msg): (string, string, int){
      $params = explode(" ",str_replace("  ", " ", $msg));
      $protocol = $params[0];
      $method = $params[1];
      $params = "/login/$method ".implode(" ",array_slice($params, 2));

      if(!is_dir(__DIR__."/verspec/$protocol/")){
        throw new InvalidMessageException("Unknown Protocol $protocol");
      }
      require_once __DIR__."/verspec/$protocol/MessageHandler.class.hh";

      $message = MessageHandler::parse($params);
      list($message, $answer) = MessageHandler::execute($message);
      $jsonAnswer = json_decode($answer);
      return tuple($protocol, $answer, intval($jsonAnswer["id"]));
    }
  }
