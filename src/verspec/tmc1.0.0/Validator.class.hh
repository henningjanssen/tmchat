<?hh // strict
  //validates the message, to have correct parameters and stuff

  require_once __DIR__.'/CommandInfo.class.hh';
  require_once __DIR__.'/DispatchedCmd.shape.hh';
  require_once __DIR__.'/../../Message.class.hh';
  require_once __DIR__.'/../../exceptions/InvalidMessageException.class.hh';

  class Validator {
    public function __construct(private DispatchedCmd $cmd) {}

    public function validate(): Message {
      $this->cmd["path"] = CommandInfo::resolveAlias($this->cmd["path"]);
      if(!CommandInfo::pathExists($this->cmd["path"])){
        $this->error("Unknown path '".$this->cmd['path']."'");
      }

      $paramInfo = CommandInfo::getParamInfo($this->cmd['path']);
      if($this->cmd['parameters']->count() < $paramInfo['minParamCount']){
        $this->error("Too few parameters");
      }

      foreach($paramInfo['replaceKeys'] as $old => $new){
        $value = $this->cmd['parameters'][$old];
        $this->cmd['parameters'][$new] = $value;
        $this->cmd['parameters']->remove($old);
      }

      foreach($paramInfo['requiredKeys'] as $key){
        if(!$this->cmd['parameters']->contains($key)){
          $this->error("Parameter '$key' missing");
        }
      }

      foreach($paramInfo["optionals"] as $opt => $val){
        if(!$this->cmd['parameters']->contains($opt)){
          $this->cmd['parameters'][$opt] = $val;
        }
      }

      return new Message($this->cmd['path'], $this->cmd['parameters']);
    }

    private function error(string $msg = ""): void {
      throw new InvalidMessageException($msg);
    }
  }
