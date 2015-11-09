<?hh // strict
  require_once 'exceptions/NotAvailableException.class.hh';

  class Message {
    private Map<string, string> $notifications = Map{};
    private Vector<int> $recipients = Vector{};

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
