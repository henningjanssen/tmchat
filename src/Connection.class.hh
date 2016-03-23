<?hh // partial

require_once 'exceptions/ClientRejectedException.class.hh';

class Connection {
  private bool $alive = true;
  private int $timeout;
  private Vector<Message> $pendingMsgs = Vector{};
  public function __construct(
    private resource $socket, private int $ttl, private string $protocol
  ){
    if(!is_dir(__DIR__."/verspec/$protocol/")){
      throw new ClientRejectedException("Unknown Protocol $protocol");
    }
    require_once __DIR__."/verspec/$protocol/MessageHandler.class.hh";
    $this->timeout = time() + $ttl;
    $this->update(); //invalidates if necessary
  }

  public function __destruct() {
    $this->close();
  }

  public function close(): void {
    socket_write($this->socket, MessageHandler::getClosingMessage());
    socket_close($this->socket);
  }

  public function isAlive(): bool {
    return $this->alive;
  }

  public function update(): void {
    if(!$this->alive){
      throw new NotAvailableException();
    }
    $read = "";

    while(true){
      $r = socket_read($this->socket, 512);
      if($r === false){
        break;
      }
      $read .= $r;
    }
    if(socket_last_error($this->socket) == SOCKET_EPIPE){ //broken pipe
      $this->alive = false;
      socket_close($this->socket);
      return;
    }
    socket_clear_error($this->socket);
    if($read !== ''){
      $messages = MessageHandler::parse($read);
      list($messages, $ans) = MessageHandler::execute($messages);
      socket_write($this->socket, $ans);

      //TODO: check write-errors
      socket_clear_error($this->socket);

      foreach($messages as $msg){
        $this->pendingMsgs->add($msg);
      }
      $this->timeout = time() + $this->ttl;
    }
    if($this->timeout < time()){
      $this->alive = false;
      $this->close();
    }
  }

  public function getMessages(): Vector<Message> {
    $pending = $this->pendingMsgs;
    $this->pendingMsgs->clear();
    return $pending;
  }

  public function notify(Vector<Message> $msgs): void {
    foreach($msgs as $msg) {
      try{
        $not = MessageHandler::generateNotification($msg);
        socket_write($this->socket, $not);

        //TODO: check write errors
        socket_clear_error($this->socket);
      }
      catch(NotAvailableException $nae) {}
    }
  }
}
