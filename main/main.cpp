// #include <cstdint>
// #include <farfler/network/network.hpp>
// #include <farfler/network/types.hpp>
// #include <iostream>
// #include <string>
// #include <thread>
// #include <unordered_map>
// #include <vector>

// #include "boost/asio.hpp"

// using namespace farfler::network;

// int main(int argc, char *argv[]) {
//   if (argc != 2) {
//     std::cerr << "Usage: " << argv[0] << " <node_name>" << std::endl;
//     return 1;
//   }

//   boost::asio::io_context io_context;
//   Network node(io_context, argv[1]);

//   std::thread t([&io_context]() { io_context.run(); });

//   std::cout << "Commands:" << std::endl;
//   std::cout
//       << "  publish <offline/online/all> <topic> <message_type> <message_data>"
//       << std::endl;
//   std::cout << "  subscribe <offline/online/all> <topic> <message_type>"
//             << std::endl;
//   std::cout << "  unsubscribe <offline/onli ne/all> <subscription_id>"
//             << std::endl;
//   std::cout << "  exit" << std::endl;

//   std::unordered_map<std::string, std::pair<Subscription, std::string>>
//       subscriptions;

//   std::string input;
//   while (std::getline(std::cin, input)) {
//     std::istringstream iss(input);
//     std::string command, scope, topic, message_type;

//     if (!(iss >> command)) {
//       std::cout << "Invalid command. Use 'publish', 'subscribe', "
//                    "'unsubscribe', or 'exit'."
//                 << std::endl;
//       continue;
//     }

//     if (command == "publish") {
//       if (!(iss >> scope >> topic >> message_type)) {
//         std::cout
//             << "Invalid publish command. Use: publish <offline/online/all> "
//                "<topic> <message_type> <message_data>"
//             << std::endl;
//         continue;
//       }

//       if (message_type == "vector3") {
//         float x, y, z;
//         if (!(iss >> x >> y >> z)) {
//           std::cout << "Invalid vector3 format. Use: x y z" << std::endl;
//           continue;
//         }
//         Vector3 msg(x, y, z);
//         if (scope == "offline")
//           Network::PublishOffline(topic, msg);
//         else if (scope == "online")
//           Network::PublishOnline(topic, msg);
//         else if (scope == "all")
//           Network::Publish(topic, msg);
//         else
//           std::cout << "Invalid scope. Use 'offline', 'online', or 'all'."
//                     << std::endl;
//       } else if (message_type == "coordinate") {
//         double lat, lon;
//         if (!(iss >> lat >> lon)) {
//           std::cout << "Invalid coordinate format. Use: latitude longitude"
//                     << std::endl;
//           continue;
//         }
//         Coordinate msg(lat, lon);
//         if (scope == "offline")
//           Network::PublishOffline(topic, msg);
//         else if (scope == "online")
//           Network::PublishOnline(topic, msg);
//         else if (scope == "all")
//           Network::Publish(topic, msg);
//         else
//           std::cout << "Invalid scope. Use 'offline', 'online', or 'all'."
//                     << std::endl;
//       } else {
//         std::cout << "Unsupported message type. Use 'vector3' or 'coordinate'."
//                   << std::endl;
//       }
//     } else if (command == "subscribe") {
//       if (!(iss >> scope >> topic >> message_type)) {
//         std::cout << "Invalid subscribe command. Use: subscribe "
//                      "<offline/online/all> <topic> <message_type>"
//                   << std::endl;
//         continue;
//       }

//       Subscription subscription;
//       if (message_type == "vector3") {
//         auto callback = [topic](const Vector3 &message) {
//           std::cout << "Received vector3 on " << topic << ": (" << message.x
//                     << ", " << message.y << ", " << message.z << ")"
//                     << std::endl;
//         };

//         if (scope == "offline")
//           subscription = Network::SubscribeOffline(topic, callback);
//         else if (scope == "online")
//           subscription = Network::SubscribeOnline(topic, callback);
//         else if (scope == "all")
//           subscription = Network::Subscribe(topic, callback);
//         else {
//           std::cout << "Invalid scope. Use 'offline', 'online', or 'all'."
//                     << std::endl;
//           continue;
//         }
//       } else if (message_type == "coordinate") {
//         auto callback = [topic](const Coordinate &message) {
//           std::cout << "Received coordinate on " << topic << ": ("
//                     << message.latitude << ", " << message.longitude << ")"
//                     << std::endl;
//         };

//         if (scope == "offline")
//           subscription = Network::SubscribeOffline(topic, callback);
//         else if (scope == "online")
//           subscription = Network::SubscribeOnline(topic, callback);
//         else if (scope == "all")
//           subscription = Network::Subscribe(topic, callback);
//         else {
//           std::cout << "Invalid scope. Use 'offline', 'online', or 'all'."
//                     << std::endl;
//           continue;
//         }
//       } else {
//         std::cout << "Unsupported message type. Use 'vector3' or 'coordinate'."
//                   << std::endl;
//         continue;
//       }

//       subscriptions[subscription.id_] = std::make_pair(subscription, topic);
//       std::cout << "Subscribed to " << topic << " with ID: " << subscription.id_
//                 << std::endl;
//     } else if (command == "unsubscribe") {
//       if (!(iss >> scope >> topic)) {
//         std::cout << "Invalid unsubscribe command. Use: unsubscribe "
//                      "<offline/online/all> <subscription_id>"
//                   << std::endl;
//         continue;
//       }

//       std::string subscription_id = topic;
//       auto it = subscriptions.find(subscription_id);
//       if (it == subscriptions.end()) {
//         std::cout << "No subscription found with ID: " << subscription_id
//                   << std::endl;
//         continue;
//       }

//       const auto &[subscription, topic] = it->second;

//       if (scope == "offline")
//         Network::UnsubscribeOffline(topic, subscription);
//       else if (scope == "online")
//         Network::UnsubscribeOnline(topic, subscription);
//       else if (scope == "all")
//         Network::Unsubscribe(topic, subscription);
//       else {
//         std::cout << "Invalid scope. Use 'offline', 'online', or 'all'."
//                   << std::endl;
//         continue;
//       }

//       std::cout << "Unsubscribed from " << topic
//                 << " with ID: " << subscription_id << std::endl;
//       subscriptions.erase(it);
//     } else if (command == "exit") {
//       break;
//     } else if (command == "testing") {
//       struct barometer {
//         double pressure;
//         double temperature;
//       };

//       Network::SubscribeOnline("barometer", [](const barometer &message) {
//         std::cout << "Received barometer on barometer: " << message.pressure << " " << message.temperature
//                   << std::endl;
//       });

//       Network::PublishOnline("barometer", barometer{123.22, 22.22});


//     } else {
//       std::cout << "Invalid command. Use 'publish', 'subscribe', "
//                    "'unsubscribe', or 'exit'."
//                 << std::endl;
//     }
//   }

//   std::cout << "Exiting..." << std::endl;
//   io_context.stop();
//   t.join();

//   return 0;
// }

#include <boost/asio.hpp>
#include <farfler/network/network.hpp>
#include <farfler/network/types.hpp>
#include <iostream>
#include <thread>

using namespace farfler::network;

int main() {
  boost::asio::io_context io_context;
  Network(io_context, "node");
  std::thread t([&io_context]() { io_context.run(); });

  // Any message that contains a predefined number of bytes can be sent and received
  // These types are: int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t, float, double, bool and struct
  Network::Subscribe("temperature", [](const double &message) {
    std::cout << "Received temperature: " << message << std::endl;
  });

  Network::Publish("temperature", 22.5);

  // Dynamic messages can be sent and received using custom message classes.
  // These classes must have static Serialize and Deserialize methods on them that return a std::vector<char> and the class itself, respectively.
  Network::Subscribe("position", [](const Vector3 &message) {
    std::cout << "Received position: (" << message.x << ", " << message.y << ", " << message.z << ")" << std::endl;
  });

  Network::Publish("position", Vector3(1.0, 2.0, 3.0));

  // By default Publish and Subscribe methods are publishing and subscribing both online and offline.
  // To publish or subscribe only online or offline, use the PublishOnline, PublishOffline, SubscribeOnline and SubscribeOffline methods.
  Network::SubscribeOnline("status", [](const bool &message) {
    std::cout << "This message was received from another peer: " << message << std::endl;
  });

  Network::SubscribeOffline("status", [](const bool &message) {
    std::cout << "This message was received from current peer: " << message << std::endl;
  });

  Network::Subscribe("status", [](const bool &message) {
    std::cout << "This message was received from either current or another peer: " << message << std::endl;
  });

  // Initializing multiple networks is also possible.
  // By default the first Network object created will be the default network.

  Network network1(io_context, "network1");
  Network network2(io_context, "network2");

  Network::Publish(network1, "network1", 22.5);
  Network::Publish(network2, "network2", 22.5);

  Network::Publish("network2", 22.5); // This will publish to the firstly created network (network1)

  while(true); // to keep the session alive

  io_context.stop();
  t.join();
  return 0;
}
