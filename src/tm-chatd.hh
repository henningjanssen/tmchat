#!/usr/bin/env hhvm
<?hh // partial
  require_once 'Service.class.hh';
  function sig_handler(int $sig): void {
    switch($sig){
      case SIGKILL:
      case SIGTERM: {
        $GLOBALS["service"]->stop();
      } break;

      case SIGHUP: {
        //do restart
      } break;
    }
  }

  //TODO: error handling
  $pid = pcntl_fork();
  posix_setsid();

  pcntl_signal(SIGKILL, "sig_handler");
  pcntl_signal(SIGTERM, "sig_handler");
  pcntl_signal(SIGHUP, "sig_handler");

  $service = Service::getInstance();
  $service->run();
