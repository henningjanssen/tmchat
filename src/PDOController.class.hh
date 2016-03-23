<?hh // strict

require_once __DIR__.'/Log.class.hh';
require_once __DIR__.'/Singleton.class.hh';

type PDOSettings = shape(
  'dns' => string,
  'user' => string,
  'password' => string
);

class PDOController extends Singleton{
  private ?PDO $pdo;
  private ?PDOSettings $settings;

  public function setSettings(PDOSettings $settings): void{
    $this->settings = $settings;
  }

  public function getPDO(): PDO{
    if($this->pdo === null){
      if($this->settings === null){
        Log::e("PDO::getPDO", "PDO-Settings not loaded");
        throw new FatalConfigurationException("PDO-Settings not loaded");
      }
      $this->pdo = new PDO(
        $this->settings['dns'].";charset=utf8",
        $this->settings['user'],
        $this->settings['password']
      );
      $this->pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
    }
    return $this->pdo;
  }

  public function getStatement(string $query): PDOStatement{
    return $this->getPDO()->prepare($query);
  }
}
