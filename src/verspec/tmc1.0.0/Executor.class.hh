<?hh // strict

  require_once __DIR__."/CommandInfo.class.hh";
  require_once __DIR__."/../../Message.class.hh";

  class Executor{
    public function __construct(private Message $msg){}
    public function execute(): Message {
      $func = str_replace($this->msg->getRawCmd(), "/", "_");
      if(method_exists($this, $func)){
        { // UNSAFE
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
      $pdoinfo = CommandInfo::getPDOInfo("/login");
      $stmt = PDOController::getInstance()->getStatement($pdoinfo["stmt"]);
      $stmt->bindValue(
        $pdoinfo["values"]["user"]["pdokey"],
        $this->msg->getParams()[$pdoinfo["values"]["user"]["msgkey"]]
      );
      $stmt->execute();
      $result = $stmt->fetch(PDO::FETCH_ASSOC);
      if($result->rowCount() !== 1){
        throw new ClientRejectedException("User does not exist");
      }
      if(
        md5($this->msg->getParams()[$pdoinfo["values"]["password"]["msgkey"]])
        === $result[$pdoinfo["values"]["password"]["dbkey"]]
      ){
        throw new ClientRejectedException("Wrong password");
      }
      $this->msg->setAnswer(json_encode($result));
      return $this->msg;
    }

    private function _ping(): Message {
      $this->msg->setAnswer("pong\0");
      return $this->msg;
    }
  }
