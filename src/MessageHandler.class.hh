<?hh // strict
  require_once 'Message.class.hh';
  require_once 'ParseMessageResult.class.hh';

  class MessageHandler {
    public static function parse(Vector<string> $raw): Vector<ParseMessageResult> {
      //TODO: parse message and create message-object
      throw new Exception("unimplemented");
      return Vector{};
    }

    public static function execute(Vector<Message> $msg): Vector<Message> {
      //TODO: execute messages
      //That is: db-stuff
      //Returns the message, that need further interaction (msg send, logout...)
      throw new Exception("unimplemented");
      return Vector{};
    }
  }
