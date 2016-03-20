<?hh // strict

class NotificationGenerator {
  public function __construct(private Message $msg) {}

  public function generate(): string {
    $func = str_replace($this->msg->getRawCmd(), "/", "_");
    if(method_exists($this, $func)){
      { //UNSAFE
        $func = inst_meth($this, $func);
      }
      return $func();
    }
    else{
      return "";
    }
  }
}
