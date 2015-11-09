<?hh // strict
  require_once 'Client.class.hh';
  require_once 'Log.class.hh';
  require_once 'Message.class.hh';
  require_once 'exceptions/FatalConfigurationException.class.hh';
  require_once 'exceptions/InvalidIDException.class.hh';
  require_once 'exceptions/RessourcesExceededException.class.hh';

  class ClientContainer implements Iterator<Client> {
    private Map<int, Client> $clients = Map{};
    private int $maxConnections;
    private int $iteratorPosition = 0;

    public function __construct(int $maxConnections) {
      if($maxConnections < 1){
        Log::f(
          "Configuration",
          "You do not allow any client-connection. This service will shut down"
            ." as it has no job to do."
        );
        throw new FatalConfigurationException();
      }
      $this->maxConnections = $maxConnections;
    }

    public function add(Client $client): void {
      if($this->clients->count() >= $this->maxConnections){
        Log::e(
          "ClientContainer::add",
          "Too many clients! Can't insert client ".$client->getID()."."
          ." Please increase client-count."
        );
        throw new ResourcesExceededException("Too many clients");
      }

      if($this->clients[$client->getID()] === null){
        $this->clients[$client->getID()] = $client;
      }
      else{
        $this->clients[$client->getID()]->merge($client);
      }
    }

    public function current(): Client {
      return $this->clients[$this->iteratorPosition];
    }

    public function key(): int {
      return $this->iteratorPosition;
    }

    public function next(): void {
      if(!$this->clients->isEmpty()) {
        do {
          $this->iteratorPosition++;
          $this->iteratorPosition %= $this->maxConnections;
        } while(!$this->valid());
      }
    }

    public function rewind(): void {
      $this->iteratorPosition = 0;
      if(!$this->valid()){
        $this->next();
      }
    }

    public function spreadMessages(Vector<Message> $messages): void {
      $spread = Vector{};
      foreach($messages as $msg){
        foreach($msg->getRecipients() as $rec){
          if($spread[$rec] === null){
            $spread[$rec] = Vector{};
          }
          $spread[$rec][] = $msg;
        }
      }
      foreach($spread as $k => $v){
        if($this->clients[$k] !== null){
          $this->clients[$k]->notify($v);
        }
      }
    }

    public function valid(): bool {
      if(!$this->clients[$this->iteratorPosition]->isAlive()){
        $this->clients->removeKey($this->iteratorPosition);
      }
      return $this->clients[$this->iteratorPosition] === null;
    }
  }
