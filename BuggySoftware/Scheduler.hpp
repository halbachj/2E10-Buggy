#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

constexpr size_t MAX_TASKS = 10; // Max scheduled tasks

class Scheduler {
private:
    template<typename T>
    struct Task {
        void* object;                      // Pointer to object instance
        void (T::*methodPtr)();            // Store the correct member function pointer type
        unsigned int interval;
        unsigned long last_execution;
        
        Task() : object(nullptr), methodPtr(nullptr), interval(0), last_execution(0) {}

    Task(T* obj, void (T::*method)(), unsigned int i)
        : object(static_cast<void*>(obj)), methodPtr(method), interval(i), last_execution(0) {}

    void execute() const {
        if (object && methodPtr) {
            (static_cast<T*>(object)->*methodPtr)();
        }
    };

    private:
        using MemberFunctionPtr = void (*)(void*);

        template <typename T>
        static void methodInvoker(void* obj) {
            auto* instance = static_cast<T*>(obj);
            (instance->*instance->methodPtr)(); // Call stored method
        }

        void (Task::*methodPtr)();
    };

    Task taskList[MAX_TASKS] = {};  // Fixed-size task array
    size_t taskCount = 0;           // Number of added tasks

public:
  // Add a task (supports member functions)
  template <typename T>
  bool addTask(T* obj, void (T::*method)(), unsigned int interval) {
    if (taskCount >= MAX_TASKS) return false;
    taskList[taskCount++] = Task(obj, method, interval);
    return true;
  }

  void update() const {
    Task task;
    unsigned long current_time = millis();
    for (size_t i = 0; i < taskCount; ++i) {
      task = taskList[i];
      if (task.last_execution + task.interval > current_time) {
        taskList[i].execute(); // Execute function
      }
    }
  }
};

#endif // SCHEDULER_HPP
