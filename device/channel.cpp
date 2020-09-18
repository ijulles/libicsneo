#include "channel.h"

using namespace icsneo;


std::pair<std::vector<std::shared_ptr<Message>>, bool> Channel::getMessages()
{
    std::vector<std::shared_ptr<Message>> ret;
	bool retBool = getMessages(ret);
	return std::make_pair(ret, retBool);
}

bool Channel::getMessages(std::vector<std::shared_ptr<Message>> &container, size_t limit = 0, std::chrono::milliseconds timeout = std::chrono::milliseconds(0))
{
    //从队列中获取消息，队列中的消息会在设备收到相应的消息后写入
    // A limit of zero indicates no limit
	if(limit == 0)
		limit = (size_t)-1;

	if(limit > (pollingContainer.size_approx() + 4))
		limit = (pollingContainer.size_approx() + 4);

	if(container.size() < limit)
		container.resize(limit);

	size_t actuallyRead;
	if(timeout != std::chrono::milliseconds(0))
		actuallyRead = pollingContainer.wait_dequeue_bulk_timed(container.data(), limit, timeout);
	else
		actuallyRead = pollingContainer.try_dequeue_bulk(container.data(), limit);

	if(container.size() > actuallyRead)
		container.resize(actuallyRead);

	return true;
}

bool Channel::transmit(std::shared_ptr<Message> message) 
{
    message.network = network;
    return device->transmit(message);
}

bool Channel::transmit(std::vector<std::shared_ptr<Message>> messages) 
{
	for(auto& message : messages) 
    {

		if(!transmit(message))
			return false;
	}
	return true;
}