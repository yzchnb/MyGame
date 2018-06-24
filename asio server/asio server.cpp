// server.cpp : 定义控制台应用程序的入口点。  
// 聊天室程序，支持多个client相互聊天  

#include "stdafx.h"  

//  
// chat_server.cpp  
// ~~~~~~~~~~~~~~~  
//  
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)  
//  
// Distributed under the Boost Software License, Version 1.0. (See accompanying  
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)  
//  

#include <algorithm>  
#include <cstdlib>  
#include <deque>  
#include <iostream>  
#include <list>  
#include <set>  
#include <boost/bind.hpp>  
#include <boost/shared_ptr.hpp>  
#include <boost/enable_shared_from_this.hpp>  
#include <boost/asio.hpp>  
#include "chat_message.hpp"  

using boost::asio::ip::tcp;
using namespace std;

//----------------------------------------------------------------------  

typedef std::deque<chat_message> chat_message_queue;

//----------------------------------------------------------------------  

// 一个抽象类，用于提供聊天室成员的接口  
class chat_participant //  聊天参与者  
{
public:
	virtual ~chat_participant() {}
	virtual void deliver(const chat_message& msg) = 0; // 发言  
};

typedef boost::shared_ptr<chat_participant> chat_participant_ptr;

//----------------------------------------------------------------------  

class chat_room // 聊天室  
{
public:
	void join(chat_participant_ptr participant) // 6  
	{
		cout << __FUNCTION__ << endl;
		participants_.insert(participant);

		// 把聊天室内缓存的消息发送给新加入的成员,相当于：  
		//      chat_message_queue::const_iterator it;  
		//      for(it = recent_msgs_.begin(); it!=recent_msgs_.end(); ++it)  
		//          participant->deliver(*it);   
		std::for_each(recent_msgs_.begin(), recent_msgs_.end(),
			boost::bind(&chat_participant::deliver, participant, _1)); // 12 动态绑定  
	}

	void leave(chat_participant_ptr participant)
	{
		cout << __FUNCTION__ << endl;
		participants_.erase(participant);
	}

	 
	void deliver(const chat_message& msg , const chat_participant_ptr  participant) // 11 发言  
	{
		cout << __FUNCTION__ << endl;
		recent_msgs_.push_back(msg);
		while (recent_msgs_.size() > max_recent_msgs)
			recent_msgs_.pop_front(); // 将过时发言清出缓冲区  

									  // 将新消息发给每个聊天室成员，相当于：  
		std::set<chat_participant_ptr>::iterator it;  
		for (it = participants_.begin(); it != participants_.end(); ++it) {
			(*it)->deliver(msg);
		}
			  
		//std::for_each(participants_.begin(), participants_.end(),
			//boost::bind(&chat_participant::deliver, _1, boost::ref(msg))); // 12  
	}

private:
	std::set<chat_participant_ptr> participants_; // 当前聊天室的n个参与者：set，不能重复  
	enum { max_recent_msgs = 100 }; // 最大最近消息：缓冲区最多保存最近100条发言  
	chat_message_queue recent_msgs_; // 消息队列：deque,先到先出  
};

//----------------------------------------------------------------------  

// 在聊天室环境下，一个session就是一个成员  
class chat_session
	: public chat_participant, // 继承  
	public boost::enable_shared_from_this<chat_session> // 可以使用shared_from_this()(即shared_ptr<chat_session>)  
{
public:
	chat_session(boost::asio::io_service& io_service, chat_room& room) // 2 7  
		: socket_(io_service),
		room_(room)
	{
		cout << __FUNCTION__ << endl;
	}

	tcp::socket& socket() // 3 8  
	{
		cout << __FUNCTION__ << endl;
		return socket_;
	}

	void start() // 5  
	{
		cout << __FUNCTION__ << endl;
		room_.join(shared_from_this());  // 6 room_.join(shared_ptr<chat_session>);  
										 // async_read是事件处理一个机制，使用回调函数从而实现事件处理器方法  
										 // 本示例大量采用这个机制，也就是异步机制  
										 // 通过回调函数可以形成一个事件链，即在回调函数中设置一个新的事件与新回调函数  
		boost::asio::async_read(socket_,
			boost::asio::buffer(read_msg_.data(), chat_message::header_length), // 取buffer头部，正文字符数到read_msg_  
			boost::bind(
				&chat_session::handle_read_header, shared_from_this(), // 9 调用:shared_from_this()->handle_read_header(boost::asio::placeholders::error);  
				boost::asio::placeholders::error));
	}

	// 存buffer中的数据到read_msg_：header部分  
	void handle_read_header(const boost::system::error_code& error) // 9  
	{
		cout << __FUNCTION__ << endl;
		if (!error && read_msg_.decode_header())
		{
			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),// 取buffer文本部分到read_msg_  
				boost::bind(&chat_session::handle_read_body, shared_from_this(), // 10 调用:shared_from_this()->handle_read_body(boost::asio::placeholders::error);  
					boost::asio::placeholders::error));
		}
		else
		{
			room_.leave(shared_from_this()); // 14  
		}
	}

	// 存buffer中的数据到read_msg_：body部分  
	void handle_read_body(const boost::system::error_code& error) // 10  
	{
		cout << __FUNCTION__ << endl;
		if (!error)
		{
			room_.deliver(read_msg_ , shared_from_this()); // 11  
			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.data(), chat_message::header_length), // 取buffer头部，正文字符数到read_msg_  
				boost::bind(&chat_session::handle_read_header, shared_from_this(),// 调用:shared_from_this()->handle_read_header(boost::asio::placeholders::error);  
					boost::asio::placeholders::error));
		}
		else
		{
			room_.leave(shared_from_this());
		}
	}

	//存入数据到write_msgs_，送队列的最开始一条发言到buffer  
	void deliver(const chat_message& msg) // 12,有几个客户端调用几次  
	{
		cout << __FUNCTION__ << endl;
		bool write_in_progress = !write_msgs_.empty();
		write_msgs_.push_back(msg);
		if (!write_in_progress)
		{
			boost::asio::async_write(socket_,
				boost::asio::buffer(write_msgs_.front().data(),
					write_msgs_.front().length()), // 队列的最开始一条发言到buffer  
				boost::bind(&chat_session::handle_write, shared_from_this(), // 13 shared_from_this()->handle_write(boost::asio::placeholders::error)  
					boost::asio::placeholders::error));
		}
	}

	// 把write_msgs_数据送buffer，使客户端可以得到，递归调用自身值到write_msgs_为空  
	void handle_write(const boost::system::error_code& error) // 13,有几个客户端调用几次  
	{
		cout << __FUNCTION__ << endl;
		if (!error)
		{
			write_msgs_.pop_front();
			if (!write_msgs_.empty())
			{
				boost::asio::async_write(socket_,
					boost::asio::buffer(write_msgs_.front().data(),// 队列的最开始一条发言到buffer  
						write_msgs_.front().length()),
					boost::bind(&chat_session::handle_write, shared_from_this(),// 13 shared_from_this()->handle_write(boost::asio::placeholders::error)  
						boost::asio::placeholders::error));
			}
		}
		else
		{
			room_.leave(shared_from_this());
		}
	}

private:
	tcp::socket socket_;
	chat_room& room_;
	chat_message read_msg_; // 存从buffer读出的数据  
	chat_message_queue write_msgs_; // 欲写入buffer的数据队列，deque  
};

typedef boost::shared_ptr<chat_session> chat_session_ptr;

//----------------------------------------------------------------------  

class chat_server
{
public:
	chat_server(boost::asio::io_service& io_service, // 1  
		const tcp::endpoint& endpoint)
		: io_service_(io_service),
		acceptor_(io_service, endpoint)
	{
		cout << __FUNCTION__ << endl;
		chat_session_ptr new_session(new chat_session(io_service_, room_)); // 2  
		acceptor_.async_accept(new_session->socket(), // 3  
			boost::bind(&chat_server::handle_accept, this, new_session, // 4  this->handle_accept(new_session, boost::asio::placeholders::error);  
				boost::asio::placeholders::error));
	}
	// 有连接到来时触发，然后等待下个连接到来  
	void handle_accept(chat_session_ptr session, // 4  
		const boost::system::error_code& error)
	{
		cout << __FUNCTION__ << endl;
		if (!error)
		{
			session->start(); // 5   
			chat_session_ptr new_session(new chat_session(io_service_, room_)); // 7  
			acceptor_.async_accept(new_session->socket(), // 8  
				boost::bind(&chat_server::handle_accept, this, new_session, //this->handle_accept(new_session, boost::asio::placeholders::error);  
					boost::asio::placeholders::error));
		}
	}

private:
	boost::asio::io_service& io_service_;
	tcp::acceptor acceptor_;
	chat_room room_;
};

typedef boost::shared_ptr<chat_server> chat_server_ptr;
typedef std::list<chat_server_ptr> chat_server_list;

//----------------------------------------------------------------------  

int _tmain(int argc, _TCHAR* argv[])
{
	// 调用栈：  
	// 打开服务端并等待连接：1-3，acceptor_.async_accept（）  
	// 一个客户端进入并连接：4-8，然后等待下个客户端：acceptor_.async_accept（）；又加入一个客户端：重复调用4-8  
	// 任意客户端发言：9-13,12\13调用n次；任意客户发言，重复9-13  
	// 聊天中途又新加入一个客户端：456 12(n) 78 13(n)  
	// 一个客户端断开连接：9 14  
	// 异步机制：立刻返回，boost提供的异步函数前缀async_要求一个回调函数，回调函数只在某一事件触发时才被调用  
	// 回调函数：void your_completion_handler(const boost::system::error_code& ec);  
	try
	{
		boost::asio::io_service io_service;

		chat_server_list servers; // server列表：一个server就是一个聊天室  

		tcp::endpoint endpoint(boost::asio::ip::address_v4::from_string("127.0.0.1")/*tcp::v4()*/, 1000);
		chat_server_ptr server(new chat_server(io_service, endpoint)); // 1  
		servers.push_back(server);

		io_service.run();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}