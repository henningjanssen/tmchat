#!/usr/bin/env php
<?hh
  require_once 'Service.class.php';
  function sig_handler($sig){
    switch($sig){
      case SIGKILL:
      case SIGTERM:
        Service::$run = false;
      break;

      case SIGHUB:
        //do restart
      break;
    }
  }

  //TODO: error handling
  $pid = pcntl_fork();
  posix_setsid();

  pcntl_signal(SIGKILL, "sig_handler");
  pcntl_signal(SIGTERM, "sig_handler");
  pcntl_signal(SIGHUP, "sig_handler");

  Service::run();
