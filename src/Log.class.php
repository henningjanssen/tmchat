<?php
  class Log{
    public static $logconf = null;
    public static function resetConf(){
      self::$logconf = array(
        "logfile" => "log/tmchatd.log",
        "log" => array(
          "debug" => false,
          "error" => true,
          "fatal" => true,
          "info" => true,
          "other" => true,
          "warning" => true
        ),
        "mail" => "server@example.com",
        "notify" => array(
          "log_debug" => false,
          "log_error" => false,
          "log_fatal" => true,
          "log_info" => false,
          "log_other" => true,
          "log_warning" => false,
          "daemon_restart" => false,
          "daemon_start" => false,
          "daemon_stop" => true
        )
      );
    }
    public static function level(
      $level, $topic, $message, $uid = "unknown", $log = null, $notify = null
    ){
      if(self::$logconf == null){
        self::resetConf();
      }
      if($notify === null){
        $notify = self::$logconf["notify"]["other"];
      }
      if($log === null){
        $notify = self::$logconf["log"]["other"];
      }
      $msg = "[".date("d.m.Y H:i:s", time())."]"
        ." - [$topic]"
        ." - [$message]"
        ." - [uid: $uid]"
        ."\n";

      if($log){
        error_log($msg, 3, realpath(self::$logconf["logpath"]));
      }
      if($notify){
        mail(
          self::$logconf["mail"],
          "tmchat encountered new _ $level _ ($topic)",
          $msg,
          "From: ".self::$logconf["mail"]."\r\n"
        );
      }
    }
    public static function d($topic, $message, $uid = "unknown"){
      self::level(
        "debug",$topic, $message, $uid,
        self::$logconf["log"]["debug"],
        self::$logconf["notify"]["debug"]
      );
    }
    public static function e($topic, $message, $uid = "unknown"){
      self::level(
        "error", $topic, $message, $uid,
        self::$logconf["log"]["error"],
        self::$logconf["notify"]["error"]
      );
    }
    public static function f($topic, $message, $uid = "unknown"){
      self::level(
        "fatal", $topic, $message, $uid,
        self::$logconf["log"]["fatal"],
        self::$logconf["notify"]["fatal"]
      );
    }
    public static function i($topic, $message, $uid = "unknown"){
      self::level(
        "info", $topic, $message, $uid,
        self::$logconf["log"]["info"],
        self::$logconf["notify"]["info"]
      );
    }
    public static function w($topic, $message, $uid = "unknown"){
      self::level(
        "warning", $topic, $message, $uid,
        self::$logconf["log"]["warning"],
        self::$logconf["notify"]["warning"]
      );
    }
  }
