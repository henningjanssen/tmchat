<?hh  // strict

type LogConf = shape(
  'logfile' => string,
  'log' => LogLevels,
  'mail' => string,
  'notify' => NotifyConf
);

type LogLevels = shape(
  'debug' => bool,
  'error' => bool,
  'fatal' => bool,
  'info' => bool,
  'other' => bool,
  'warning' => bool
);

type NotifyConf = shape(
  'log_debug' => bool,
  'log_error' => bool,
  'log_fatal' => bool,
  'log_info' => bool,
  'log_other' => bool,
  'log_warning' => bool,
  'daemon_restart' => bool,
  'daemon_start' => bool,
  'daemon_stop' => bool
);

  class Log {
    public static ?LogConf $logconf = null;

    public static function getConf(): LogConf {
      if (self::$logconf === null) {
        return self::resetConf();
      }
      return self::$logconf;
    }
    public static function resetConf(): LogConf {
      self::$logconf = shape(
        'logfile' => 'log/tmchatd.log',
        'log' => shape(
          'debug' => false,
          'error' => true,
          'fatal' => true,
          'info' => true,
          'other' => true,
          'warning' => true
        ),
        'mail' => 'server@example.com',
        'notify' => shape(
          'log_debug' => false,
          'log_error' => false,
          'log_fatal' => true,
          'log_info' => false,
          'log_other' => true,
          'log_warning' => false,
          'daemon_restart' => false,
          'daemon_start' => false,
          'daemon_stop' => true
        )
      );
      return self::$logconf;
    }
    public static function level(
      string $level,  string $topic, string $message,
      ?int $uid = null, ?bool $log = null, ?bool $notify = null
    ) : void {
      if(self::$logconf == null){
        self::resetConf();
      }
      if($notify === null){
        $notify = self::getConf()['notify']['log_other'];
      }
      if($log === null){
        $notify = self::getConf()['log']['other'];
      }
      $msg = "[".date("d.m.Y H:i:s", time())."]"
        ." - [$topic]"
        ." - [$message]"
        ." - [uid: $uid]"
        ."\n";

      if($log){
        error_log($msg, 3, realpath(self::getConf()['logfile']));
      }
      if($notify){
        mail(
          self::getConf()['mail'],
          "tmchat encountered new _ $level _ ($topic)",
          $msg,
          "From: ".self::getConf()['mail']."\r\n"
        );
      }
    }
    public static function d(
      string $topic, string $message, ?int $uid = null
    ) : void {
      self::level(
        "debug",$topic, $message, $uid,
        self::getConf()['log']['debug'],
        self::getConf()['notify']['log_debug']
      );
    }
    public static function e(
      string $topic, string $message, ?int $uid = null
    ) : void {
      self::level(
        "error", $topic, $message, $uid,
        self::getConf()['log']['error'],
        self::getConf()['notify']['log_error']
      );
    }
    public static function f(
      string $topic, string $message, ?int $uid = null
    ) : void {
      self::level(
        "fatal", $topic, $message, $uid,
        self::getConf()['log']['fatal'],
        self::getConf()['notify']['log_fatal']
      );
    }
    public static function i(
      string $topic, string $message, ?int $uid = null
    ) : void {
      self::level(
        "info", $topic, $message, $uid,
        self::getConf()['log']['info'],
        self::getConf()['notify']['log_info']
      );
    }
    public static function w(
      string $topic, string $message, ?int $uid = null
    ) : void {
      self::level(
        "warning", $topic, $message, $uid,
        self::getConf()['log']['warning'],
        self::getConf()['notify']['log_warning']
      );
    }
  }
