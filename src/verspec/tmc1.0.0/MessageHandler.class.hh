<?hh // strict
  require_once __DIR__.'/Executor.class.hh';
  require_once __DIR__.'/Parser.class.hh';
  require_once __DIR__.'/../../Message.class.hh';
  require_once __DIR__.'/../../exceptions/NotAvailableException.class.hh';

  class MessageHandler {
    const VERSION = "1.0.0";

    public static function parse(string $raw): Vector<Message> {
      $seperated = new Vector(explode("\0", $raw));
      $result = Vector{};

      $seperated->removeKey($seperated->count()-1);
      foreach($seperated as $s){
        $msg = new Message("", Map{}); //gets overwritten or invalidated
        try{
          $parser = new Parser($s);
          $dispmsg = $parser->parse();
          $validator = new Validator($dispmsg);
          $msg = $validator->validate();
        }
        catch(SyntaxErrorException $seex){
          $msg->invalidate(400, $seex->getMessage());
        }
        catch(InvalidMessageException $imex){
          $msg->invalidate(421, $imex->getMessage());
        }
        $result->add($msg);
      }
      return $result;
    }

    public static function execute(Vector<Message> $msgs)
    : (Vector<Message>, string) {
      $answer = "";
      foreach($msgs as &$msg){ //I knooow it's slooow
        try{
          $ex = new Executor($msg);
          $msg = $ex->execute();
        }
        catch(NotAvailableException $naex){
          $msg->invalidate(421, $naex->getMessage());
        }
        catch(ClientRejectedException $crex){ // roooaaar
          $msg->invalidate(10001, $crex->getMessage());
        }
        $answer .= $msg->getSendableString()."\0";
      }
      return list($msgs, $answer);
    }

    public static function generateNotification(Message $msg): string {
      $not = "";
      try {
        if($msg->valid()){
          $not = $msg->getNotification(self::VERSION);
        }
      }
      catch(NotAvailableException $nae) {
        $gen = new NotificationGenerator($msg);
        $not = $gen->generate();
        $msg->setNotification(self::VERSION, $not);
      }
      if($not === ""){
        throw new NotAvailableException("No notifications needed");
      }
      return $not;
    }

    public static function getClosingMessage(): string {
      return "10002 Closing Connection";
    }
  }
