#include <queue>
#include <mutex>
#include <condition_variable>
#include <vector>

template <typename T>
class Pipeline
{
  private:
    std::queue<T> _q;
    std::mutex _m;
    std::condition_variable _cv;

  public:
    void push(const T& d)
    {
      std::unique_lock<std::mutex> lk(_m);
      _q.push(d);
      _cv.notify_all();
    }

    void push(const std::vector<T>& V)
    {
      for (auto const& v : V)
      {
        push(v);
      }
    }

    T pop()
    {
      std::unique_lock<std::mutex> lk(_m);
      if(_q.empty())
      {
        _cv.wait(lk, [=]{ return !_q.empty(); });
      }

      T data = _q.front();
      _q.pop();
      return data;
    }
};
