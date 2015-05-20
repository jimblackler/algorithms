class Timer {

public:
  static Timer* newTimer();
  virtual long long getTime() = 0;

};

