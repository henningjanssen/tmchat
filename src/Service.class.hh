<?hh // strict
  require_once 'Server.class.hh';
  require_once 'Singleton.class.hh';

  class Service extends Singleton {
    private bool $run = false;

    public function stop() : void {
      $this->run = false;
    }

    public function run() : void {
      $this->run = true;

      //load configuration
      $conff = __DIR__."/conf/conf"
        .(file_exists(__DIR__."/dev")?"-dev":"")
        .".json";
      if(!file_exists($conff)){
        echo "fatal: config-file not found: $conff";
      }
      $jconf = file_get_contents($conff);
      $conf = json_decode($jconf, true);

      //open port and accept users
      $server = new Server(
        $conf["connection"]["host"],
        $conf["connection"]["port"],
        $conf["connection"]["pem_file"],
        $conf["connection"]["pem_pass"]
      );

      while($this->run){
        $new = false;
        do{
          $new = $server->accept();
        } while($new !== null && $this->run);
      }
    }
  }
