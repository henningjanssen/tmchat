<?hh // strict
  require_once __DIR__.'/../../Message.class.hh';
  require_once __DIR__.'/../../exceptions/NotAvailableException.class.hh';

  class MessageHandler {
    const VERSION = "1.0.0";

    public static function parse(string $raw): Vector<Message> {
      throw new Exception("unimplemented");
      //TODO: split single messages by seperator
      //TODO: parse message and create message-object
      $result = Vector{};
      //TODO: seperator
      $seperated = explode("", $raw);
      foreach($seperated as $s){
        if(!self::checkSyntax($s)){
          $message = new Message();
          //TODO: parse
        }
      }
      return $result;
    }

    public static function checkSyntax(string $msg): bool {
      //TODO: real check
      return $msg[0] === '/';
    }

    public static function execute(Vector<Message> $msg): void {
      //TODO: execute messages
      //That is: db-stuff
      throw new Exception("unimplemented");
    }

    public static function generateAnswer(Vector<Message> $msg): string {
      //TODO: generate answers that are sent to the clients
      throw new Exception("unimplemented");
      return "";
    }

    public static function generateNotification(Message $msg): string {
      $not = "";
      try {
        $not = $msg->getNotification(self::VERSION);
      }
      catch(NotAvailableException $nae) {
        //TODO: generate stuff
        $msg->setNotification(self::VERSION, $not);
      }
      if($not === ""){
        throw new NotAvailableException("No notifications needed");
      }
      return $not;
    }

    public static function getClosingMessage(): string {
      //TODO: real message
      return "";
    }
  }
