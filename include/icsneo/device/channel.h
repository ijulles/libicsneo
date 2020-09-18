#ifndef __CHANNEL_H__
#define __CHANNEL_H__

#ifdef __cplusplus

#include "device.h"

namespace icsneo
{
    class Channel
    {
    public:
        virtual ~Channel();
        std::pair<std::vector<std::shared_ptr<Message>>, bool> getMessages();
        bool getMessages(std::vector<std::shared_ptr<Message>> &container, size_t limit = 0, std::chrono::milliseconds timeout = std::chrono::milliseconds(0));

        bool transmit(std::shared_ptr<Message> message);
        bool transmit(std::vector<std::shared_ptr<Message>> messages);

        size_t getCurrentMessageCount() { return pollingContainer.size_approx(); }

    private:
        std::shared_ptr<icsneo::Device> device;
        std::shared_ptr<icsneo::Network> network;
        moodycamel::BlockingConcurrentQueue<std::shared_ptr<Message>> pollingContainer;
    };

} // namespace icsneo

#endif // __cplusplus

#endif