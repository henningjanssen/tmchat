<?php
  class Log{
    public static $logconf = null;
    public static function resetConf(){
      Log::$logconf = array(
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
    public static function _log(
      $level, $topic, $message, $uid = "unknown", $log = null, $notify = null
    ){
      if(Log::$logconf == null){
        Log::resetConf();
      }
      if($notify === null){
        $notify = Log::$logconf["notify"]["other"];
      }
      if($log === null){
        $notify = Log::$logconf["log"]["other"];
      }
      $msg = "[".date("d.m.Y H:i:s", time())."]"
        ." - [$topic]"
        ." - [$message]"
        ." - [uid: $uid]"
        ."\n";

      if($log){
        error_log($msg, 3, realpath(Log::$logconf["logpath"]));
      }
      if($notify){
        mail(
          Log::$logconf["mail"],
          "tmchat encountered new _ $level _ ($topic)",
          $msg,
          "From: ".Log::$logconf["mail"]."\r\n"
        );
      }
    }
    public static function d($topic, $message, $uid = "unknown"){
      Log::log(
        "debug",$topic, $message, $uid,
        Log::$logconf["log"]["debug"],
        Log::$logconf["notify"]["debug"]
      );
    }
    public static function e($topic, $message, $uid = "unknown"){
      Log::log(
        "error", $topic, $message, $uid,
        Log::$logconf["log"]["error"],
        Log::$logconf["notify"]["error"]
      );
    }
    public static function f($topic, $message, $uid = "unknown"){
      Log::log(
        "fatal", $topic, $message, $uid,
        Log::$logconf["log"]["fatal"],
        Log::$logconf["notify"]["fatal"]
      );
    }
    public static function i($topic, $message, $uid = "unknown"){
      Log::log(
        "info", $topic, $message, $uid,
        Log::$logconf["log"]["info"],
        Log::$logconf["notify"]["info"]
      );
    }
    public static function w($topic, $message, $uid = "unknown"){
      Log::log(
        "warning", $topic, $message, $uid,
        Log::$logconf["log"]["warning"],
        Log::$logconf["notify"]["warning"]
      );
    }
  }
