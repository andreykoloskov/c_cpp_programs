namespace RAII {

template<typename T>
class Booking {
public:
  Booking(T* provider, int counter)
      : provider_(provider), counter_(counter), internal_counter_(1) {
  }
  
  Booking(const Booking& other)
      : provider_(other.provider_)
      , counter_(other.counter_)
      , internal_counter_(other.internal_counter_) {
    ++internal_counter_;
  }

  Booking& operator=(const Booking& other) {
    if (&other != this) {
      provider_ = other.provider_;
      counter_ = other.counter_;
      internal_counter_ = other.internal_counter_;
      ++internal_counter_;
    }
    return *this;
  }

  Booking(Booking&& other)
      : provider_(other.provider_)
      , counter_(other.counter_)
      , internal_counter_(other.internal_counter_) {
    other.provider_ = nullptr;
    other.counter_ = 0;
    other.internal_counter_ = 0;
  }

  Booking& operator=(Booking&& other) {
    if (&other != this) {
      provider_ = other.provider_;
      counter_ = other.counter_;
      internal_counter_ = other.internal_counter_;
      other.provider_ = nullptr;
      other.counter_ = 0;
      other.internal_counter_ = 0;
    }
    return *this;
  }

  ~Booking() {
    --internal_counter_;
    if (internal_counter_ == 0 && provider_) {
      provider_->CancelOrComplete(*this);
    }
  }

private:
  T* provider_;
  int counter_;
  int internal_counter_;
};

}