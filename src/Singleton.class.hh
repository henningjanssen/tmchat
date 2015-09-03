<?hh
  <<__ConsistentConstruct>>
  class Singleton{
    protected static $instance = null;

    public static function getInstance(): this {
      if(static::$instance === null){
        static::$instance = new static();
      }
      return static::$instance;
    }

    protected function __construct() {}
    protected function __clone(): void {}
    protected function __wakeup(): void {}
  }
