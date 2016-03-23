<?hh

class LibConf extends Singleton{
  private static Map<string, Map<string, mixed>> $conf = Map{};

  public static function init(): void {
    if(self::$conf->count() > 0){
      return;
    }
    if(!file_exists(__DIR__."/conf/libconf.json")){
      return;
    }
    $json = file_get_contents(__DIR__."/conf/libconf.json");
    self::$conf = json_decode($json, true);
  }

  public static function confExists(string $libname){
    self::init();
    return self::$conf->contains($libname);
  }

  public static function getConf(string $libname){
    self::init();
    return self::$conf[$libname];
  }
}
