<?hh // strict
  require_once 'exceptions/NotAvailableException.class.hh';

  class Message {
    private Map<string, string> $notifications = Map{};
    private Vector<int> $recipients = Vector{};
    private int $error = 0;
    private string $errstr = "";
    private string $answer = "";

    public function __construct(
      private string $rawcmd, private Map<string, string> $params){}

    public function setAnswer(string $answ): void {
      $this->answer = $answ;
    }

    public function getAnswer(): string {
      return $this->answer;
    }

    public function getRawCmd(): string {
      return $this->rawcmd;
    }

    public function invalidate(int $error, string $errstr): void {
      $this->error = $error;
      $this->errstr = $errstr;
    }
    public function valid(): bool {
      return $this->error === 0 || $this->error === 200;
    }

    public function setRecipients(Vector<int> $rec): void {
      $this->recipients = $rec;
    }

    public function getRecipients(): Vector<int> {
      return $this->recipients;
    }

    public function getSendableString(): string {
      throw new Exception("unimplemented");
      return "";
    }

    public function getNotification(string $version): string {
      if(!$this->notifications->contains($version)) {
        throw new NotAvailableException();
      }
      return $this->notifications[$version];
    }

    public function setNotification(string $version, string $notif): void {
      $this->notifications[$version] = $notif;
    }
  }
