#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>


template <typename T>
class SharedQueue
{
public:
    SharedQueue();
    ~SharedQueue();

    T &front();
    void pop_front();
//    int front_check(QueuePacket &current_packet, int input_slave_number);
//    int front_check(QueuePacket &current_packet, int input_slave_number_1, int input_slave_number_2, int input_slave_number_3, int input_slave_number_4);

    void push_back(const T &item);
    void push_back(T &&item);


    int size();

private:
    std::deque<T> queue_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

template <typename T>
SharedQueue<T>::SharedQueue() = default;

template <typename T>
SharedQueue<T>::~SharedQueue() = default;

template <typename T>
T &SharedQueue<T>::front()
{
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
        cond_.wait(mlock);
    }

    return queue_.front();
}

//template <class T>
//struct TypeIsQueuePacket
//{
//    static const bool value = false;
//};

//template <>
//struct TypeIsQueuePacket<QueuePacket>
//{
//    static const bool value = true;
//};



//template <typename T>
//int SharedQueue<T>::front_check(QueuePacket &current_packet, int input_slave_number_1,int input_slave_number_2,int input_slave_number_3,int input_slave_number_4)
//{
//    std::unique_lock<std::mutex> mlock(mutex_);
//
//    /* check if the queue is empty or not*/
//    if(queue_.empty()){
//        return -2; /*EMPTY code*/
//    }
//
//    if (TypeIsQueuePacket<T>::value)
//    {
//        current_packet = queue_.front();
//
//        int current_slave_number = current_packet.getDestinationNumber();
//
//        if (current_slave_number == input_slave_number_1)
//        {
//            queue_.pop_front();
//            // current packet is mine and everything is OK
//            return input_slave_number_1;
//        }
//        else if(current_slave_number == input_slave_number_2)
//        {
//            queue_.pop_front();
//            // current packet is mine and everything is OK
//            return input_slave_number_2;
//
//        }else if(current_slave_number == input_slave_number_3)
//
//        {queue_.pop_front();
//            // current packet is mine and everything is OK
//            return input_slave_number_3;
//        }else if(current_slave_number == input_slave_number_4)
//        {
//            queue_.pop_front();
//            // current packet is mine and everything is OK
//            return input_slave_number_4;
//        }
//        else
//        {
//            // It is not for this slave so check ttl and then go back to the slave's work
//
//            // check ttl
//            if (current_packet.getTTL() < 0)
//            {
//                queue_.pop_front();
//            }
//            else
//            {
//                current_packet.reduceTTL();
//            }
//
//            return -1;
//        }
//    }
//    else
//    {
//        // It is not happen in this program
//        return -1000;
//    }
//
//}




//template <typename T>
//int SharedQueue<T>::front_check(QueuePacket &current_packet, int input_slave_number)
//{
//    std::unique_lock<std::mutex> mlock(mutex_);
//
//    /* check if the queue is empty or not*/
//    if(queue_.empty()){
//        return -2; /*EMPTY code*/
//    }
//
//    if (TypeIsQueuePacket<T>::value)
//    {
//        current_packet = queue_.front();
//
//
//        int current_slave_number = queue_.front().getDestinationNumber();
//
//
//        if (current_slave_number == input_slave_number)
//        {
//            queue_.pop_front();
//            // current packet is mine and everything is OK
//            return 1;
//        }
//        else
//        {
//            // It is not for this slave so check ttl and then go back to the slave's work
//
//            // check ttl
//            if (queue_.front().getTTL() < 0)
//            {
//                queue_.pop_front();
//            }
//            else
//            {
//                queue_.front().reduceTTL();
//
//            }
//
//            return -1;
//        }
//    }
//    else
//    {
//        // It is not happen in this program
//        return -1000;
//    }
//}

template <typename T>
void SharedQueue<T>::pop_front()
{
    std::unique_lock<std::mutex> mlock(mutex_);
    while (queue_.empty())
    {
        cond_.wait(mlock);
    }
    queue_.pop_front();
}

template <typename T>
void SharedQueue<T>::push_back(const T &item)
{
    std::unique_lock<std::mutex> mlock(mutex_);
    queue_.push_back(item);
    mlock.unlock();     // unlock before notificiation to minimize mutex con
    cond_.notify_one(); // notify one waiting thread
}

//template <typename T>
//void SharedQueue<T>::push_back(T &&item)
//{
//    std::unique_lock<std::mutex> mlock(mutex_);
//    queue_.push_back(std::move(item));
//    if(TypeIsQueuePacket<T>::value){
//        QueuePacket currentPacket = queue_.front();
////        std::cout << "[PUSH BACK] : "<< currentPacket.getMessage() << std::endl;
//
//    }
//    mlock.unlock();     // unlock before notificiation to minimize mutex con
//    cond_.notify_one(); // notify one waiting thread
//}

template <typename T>
int SharedQueue<T>::size()
{
    std::unique_lock<std::mutex> mlock(mutex_);
    int size = queue_.size();
    mlock.unlock();
    return size;
}