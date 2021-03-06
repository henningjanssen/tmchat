<?hh // partial

  require_once __DIR__."/CommandInfo.class.hh";
  require_once __DIR__."/../../LibConf.class.hh";
  require_once __DIR__."/../../Message.class.hh";
  require_once __DIR__."/../../../vendor/autoload.php";

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
          "Command ".$this->msg->getRawCmd()." does not exist", 501
        );
      }
    }

    private function _login_mail(): Message {
      $pdoinfo = CommandInfo::getPDOInfo("/login");
      $stmt = PDOController::getInstance()->getStatement($pdoinfo["stmt"]);
      $stmt->bindValue(
        $pdoinfo["values"]["user"]["pdokey"],
        $this->msg->getParams()[$pdoinfo["values"]["user"]["msgkey"]],
        self::getPDOType($pdoinfo["values"]["user"]["type"])
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
      $this->msg->invalidate(200, "OK");
      $this->msg->setAnswer(json_encode($result));
      return $this->msg;
    }

    private function _login_fb(): Message {
      $pdoinfo = CommandInfo::getPDOInfo("/login/fb");
      $accessToken = intval($this->msg->getParams()["accessToken"]);
      $fbConf = LibConfiguration::getConf("facebook");

      $fb = new Facebook(array(
        "appID" => $fbConf["appID"],
        "secret" => $fbConf["appSecret"]
      ));
      $fb->setAccessToken($accessToken);

      if(!$fb->getUser()){
        throw new ClientRejectedException(
          "Failed to get facebook-user"
        );
      }
      $fbUser = null;
      try{
        $fbUser = $fb->api("/me", "GET");
      }
      catch(Exception $ex){
        throw new ClientRejectedException(
          "Failed to access facebook-profile",
          10001, $ex
        );
      }
      $stmt = PDOController::getInstance()->getStatement($pdoinfo["stmt"]);
      $stmt->bindValue(
        $pdoinfo["values"]["fbid"]["dbkey"],
        $accessToken,
        self::getPDOType($pdoinfo["values"]["fbid"]["type"])
      );
      $stmt->execute();
      $result = $stmt->fetch(PDO::FETCH_ASSOC);
      if($result->rowCount() !== 1){
        throw new ClientRejectedException("User does not exist");
      }
      $this->msg->invalidate(200, "OK");
      $this->msg->setAnswer(json_encode($result));
      return $this->msg;
    }

    private function _ping(): Message {
      $this->msg->invalidate(200, "OK");
      $this->msg->setAnswer("pong\0");
      return $this->msg;
    }

    private static function getPDOType(string $typename): int{
      switch(strtolower($typename)){
        case "bool":
          return PDO::PARAM_BOOL;
          break;

        case "int":
          return PDO::PARAM_INT;
          break;

        case "null":
          return PDO::PARAM_NULL;
          break;

        case "string":
          return PDO::PARAM_STR;
          break;

        case "lob":
          return PDO::PARAM_LOB;
          break;

        case "stmt":
          return PDO::PARAM_STMT;
          break;

        case "input-output":
          return PDO::PARAM_INPUT_OUTPUT;
          break;

        default:
          Log::e("tmc1.0.0: Executor::getType", "Unknown type $typename");
          throw new FatalConfigurationException("Unknown type $typename");
      }
    }
  }
