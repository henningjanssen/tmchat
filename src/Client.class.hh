<?hh // strict
  require_once 'Message.class.hh';
  require_once 'exceptions/InvalidIDException.class.hh';
  require_once 'shapes/ParseMessageResult.shape.hh';

  type Socket = shape (
    'sock' => resource,
    'timeout' => int
  );

  class Client{
    private int $id;
    private Vector<Socket> $socks = Vector{};

    public function __construct(int $id, private int $timeout) {
      if($id <= 0){
        throw new InvalidIDException();
      }
      $this->id = $id;
    }

    public function addConnection(resource $sock): void {
      //TODO: check if alive
      $this->socks[] = shape(
        'sock' => $sock,
        'timeout' => time() + $this->timeout,
      );
    }

    public function getConnectionCount(): int {
      return $this->socks->count();
    }

    public function getID(): int {
      return $this->id;
    }

    public function getSocks(): ImmVector<Socket> {
      return $this->socks->toImmVector();
    }

    public function handleConnections(): Vector<Message> {
      $messages = Vector{};
      //TODO: iterate through connections and handle their messages.
      //TODO: respond
      //TODO: rm connections that timeouted
      throw new Exception("unimplemented");
      foreach($this->socks->keys() as $k){
        $this->socks[$k]['timeout'] = time() + $this->timeout;
      }
      return $messages;
    }

    public function isAlive(): bool {
      return !($this->socks->isEmpty());
    }

    public function merge(Client $client): void {
      foreach($client->getSocks() as $sock){
        $this->addConnection($sock['sock']);
      }
    }

    public function notify(Vector<Message> $messages): void {
      $sends = Vector{};
      throw new Exception("unimplemented");
      foreach($messages as $msg){
        $sends[] = $msg->getSendableString();
      }
      foreach($this->socks as $sock){
        foreach($sends as $send){
          fwrite($sock, $send);
          //TODO: get answer and stuff
        }
      }
    }
  }
