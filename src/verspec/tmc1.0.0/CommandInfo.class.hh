<?hh // strict

  require_once __DIR__
    .'/../../exceptions/FatalConfigurationExceptions.class.hh';

  type PDOInfo = shape(
    'stmt' => string,
    'values' => Map<string, shape(
        "pdokey" => string,
        "msgkey" => string,
        "dbkey" => string,
        "type" => string
      )>
  );
  type ParamInfo = shape(
    'minParamCount' => int,
    'requiredKeys' => Vector<string>,
    'replaceKeys' => Map<string, string>, //Replace unkeyed param i with key k
    'optionals' => Map<string, string>
  );
  class CommandInfo {
    private static Map<string, string> $pathAliases = Map{};
    private static Map<string, ParamInfo> $info = Map{};
    private static Map<string, PDOInfo> $pdo = Map{};

    public static function init(): void{
      if(self::$pathAliases->count() === 0){
        $json = self::loadInfo("json/pdo.json");
        $jsondec = json_decode($json);
        if($jsondec === null){
          throw new FatalConfigurationException(
            "Invalid json: ".__DIR__."/json/pdo.json"
          );
        }
        self::$pdo = $jsondec;
      }
      if(self::$info->count() === 0){
        $json = self::loadInfo("json/pdo.json");
        $jsondec = json_decode($json);
        if($jsondec === null){
          throw new FatalConfigurationException(
            "Invalid json: ".__DIR__."/json/pdo.json"
          );
        }
        self::$info = $jsondec;
      }
      if(self::$pdo->count() === 0){
        $json = self::loadInfo("json/pdo.json");
        $jsondec = json_decode($json);
        if($jsondec === null){
          throw new FatalConfigurationException(
            "Invalid json: ".__DIR__."/json/pdo.json"
          );
        }
        self::$pdo = $jsondec;
      }
    }

    private static function loadInfo(string $file): string{
      if(!file_exists(__DIR__."/$file")){
        throw new FatalConfigurationException(
          "File not found: ".__DIR__."/$file"
        );
      }
      $filec = file_get_contents(__DIR__."/$file");
      return $filec;
    }

    public static function pathExists(string $path): bool{
      self::init();
      $info = self::$info;
      $pathAliases = self::$pathAliases;
      return $info->contains($path) || $pathAliases->contains($path);
    }

    public static function getParamInfo(string $path): ParamInfo{
      self::init();
      return self::$info[$path];
    }

    public static function getPDOInfo(string $path): PDOInfo{
      self::init();
      return self::$pdo[$path];
    }

    public static function resolveAlias(string $path): string{
      if(self::$pathAliases->contains($path)){
        return self::$pathAliases[$path];
      }
      return $path;
    }
  }
