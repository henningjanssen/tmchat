<?hh // strict

  require_once __DIR__."/CommandInfo.class.hh";
  require_once __DIR__."/../../Message.class.hh";

  class Executor{
    public function __construct(private Message $msg){}
    public function execute(): Message {
      $func = str_replace($this->msg->getRawCmd(), "/", "_");
      if(method_exists($this, $func)){
        { //UNSAFE
          $func = inst_meth($this, $func);
        }
        return $func();
      }
      else{
        throw new NotAvailableException(
          "Command ".$this->msg->getRawCmd()." does not exist"
        );
      }
    }

    private function _login_mail(): Message {
      throw new NotAvailableException("/login/mail is unimplemented");
      return $this->msg;
    }

    private function _ping(): Message {
      $this->msg->setAnswer("pong\0");
      return $this->msg;
    }
  }
