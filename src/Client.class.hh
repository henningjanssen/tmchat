<?hh // strict
  require_once 'Connection.class.hh';
  require_once 'Message.class.hh';
  require_once 'exceptions/ClientRejectedException.class.hh';
  require_once 'exceptions/InvalidIDException.class.hh';

  class Client{
    private int $id;
    private Vector<Connection> $connections = Vector{};

    public function __construct(int $id, private int $timeout) {
      if($id <= 0){
        throw new InvalidIDException();
      }
      $this->id = $id;
    }

    public function addSock(resource $sock, string $protocol): void {
      $this->connections[] = new Connection($sock, $this->timeout, $protocol);
    }

    public function addConnection(Connection $connection): void {
      if($connection->isAlive()){
        $this->connections[] = $connection;
      }
    }

    public function getConnectionCount(): int {
      return $this->connections->count();
    }

    public function getID(): int {
      return $this->id;
    }

    public function getConnections(): ImmVector<Connection> {
      return $this->connections->toImmVector();
    }

    public function handleConnections(): Vector<Message> {
      $messages = Map{};
      $msgVector = Vector{};
      $rm = Vector{};
      foreach($this->connections as $k => $v){
        $v->update();
        if(!$v->isAlive()){
          continue;
        }
        $messages[$k] = $v->getMessages();
      }
      foreach($messages->keys() as $mk){
        foreach($this->connections->keys() as $ck){
          if($mk != $ck){
            $this->connections[$ck]->notify($messages[$mk]);
          }
          //TODO: improve efficiency
          foreach($messages[$mk] as $sm){
            if(!$sm->getRecipients()->isEmpty()){
              $msgVector[] = $sm;
            }
          }
        }
      }
      $this->connections = $this->connections->filter(
        $conn ==> $conn->isAlive()
      );

      return $msgVector;
    }

    public function isAlive(): bool {
      return !($this->connections->isEmpty());
    }

    public function merge(Client $client): void {
      if($client->getID() !== $this->id){
        throw new InvalidIDException("IDs don't match");
      }
      foreach($client->getConnections() as $conn){
        $this->addConnection($conn);
      }
    }

    public function notify(Vector<Message> $messages): void {
      foreach($this->connections as $conn){
        $conn->notify($messages);
      }
    }
  }
