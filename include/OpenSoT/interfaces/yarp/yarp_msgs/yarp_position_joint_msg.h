#ifndef __INTERFACES_YARP_POSITION_JOINT_MSG_H__
#define __INTERFACES_YARP_POSITION_JOINT_MSG_H__

#include <string>
#include <yarp/os/all.h>
#include <yarp/sig/all.h>
#include <map>

namespace OpenSoT {
    namespace interfaces {
        namespace yarp {
            namespace msgs{

            /**
             * @brief The yarp_position_joint_msg class define a vector of joint position, velocities and names
             */
            class yarp_position_joint_msg
            {
            public:
                /**
                 * @brief yarp_position_joint_msg creates an empty map
                 */
                yarp_position_joint_msg():
                joints()
                {}

                /**
                 * @brief yarp_position_joint_msg constructor that assign a map
                 * @param joints_map
                 */
                yarp_position_joint_msg(const std::map<std::string, double>& joints_map):
                joints(joints_map)
                {}

                /**
                 * @brief joints is a map <joint_name, joint_value>, joint values are in [rad]!
                 */
                std::map<std::string, double> joints;

                /**
                 * @brief bottle_msg_size contains the lenght of the bottle created by the serializeMsg.
                 */
                const unsigned int bottle_msg_size(){return 2*joints.size();}

                /**
                 * @brief serializeMsg push in a bottle data contained in the std::map. Data are serialized as:
                 * [name0, j0, name1, j1, ..., namen, jn]
                 * @param tmp_b bottle
                 */
                void serializeMsg(::yarp::os::Bottle& tmp_b)
                {
                    for(std::map<std::string, double>::iterator i = joints.begin(); i != joints.end(); ++i)
                    {
                        tmp_b.addString(i->first);
                        tmp_b.addDouble(i->second);                        
                    }
                }

                /**
                 * @brief deserializeMsg write on a std::map the data contained in the bottle
                 * @param tmp_b bottle
                 * @param i index from which data are started to read
                 */
                void deserializeMsg(::yarp::os::Bottle& tmp_b, unsigned int i = 0)
                {
                    if(tmp_b.size() >= 2)
                    {
                        unsigned int j = 0;
                        for(unsigned int k = 0; k < tmp_b.size()/2; ++k)
                        {
                            joints[tmp_b.get(j+i).asString()] = tmp_b.get(j+1+i).asDouble();
                            j = j+2;
                        }
                    }
                }

            };

            /**
             * @brief The yarp_position_joint_msg_portable class implement a portable from yarp_position_joint_msg
             */
            class yarp_position_joint_msg_portable: public yarp_position_joint_msg, public ::yarp::os::Portable
            {
            public:
                yarp_position_joint_msg_portable():
                yarp_position_joint_msg()
                {

                }

                yarp_position_joint_msg_portable(const std::map<std::string, double>& joints_map):
                yarp_position_joint_msg(joints_map)
                {

                }

                bool read(::yarp::os::ConnectionReader& connection)
                {
                    ::yarp::os::Bottle tmp_bot;
                    if(!tmp_bot.read(connection))
                        return false;

                    deserializeMsg(tmp_bot);
                    return true;
                }

                bool write(::yarp::os::ConnectionWriter& connection)
                {
                    ::yarp::os::Bottle tmp_bot;
                    serializeMsg(tmp_bot);

                    if(tmp_bot.write(connection)){
                        return true;}
                    return false;
                }
            };

            }
        }
    }
}

#endif
