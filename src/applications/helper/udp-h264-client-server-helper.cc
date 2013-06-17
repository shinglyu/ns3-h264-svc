/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2008 INRIA
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mohamed Amine Ismail <amine.ismail@sophia.inria.fr>
 */
//#include "udp-client-server-helper.h"
#include "udp-h264-client-server-helper.h"
#include "ns3/udp-server.h"
#include "ns3/udp-h264-server.h"
#include "ns3/udp-client.h"
//#include "ns3/udp-trace-client.h"
#include "ns3/udp-h264-trace-client.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"

namespace ns3 {

UdpH264ServerHelper::UdpH264ServerHelper ()
{
}

UdpH264ServerHelper::UdpH264ServerHelper (uint16_t port)
{
  m_factory.SetTypeId (UdpH264Server::GetTypeId ());
  SetAttribute ("Port", UintegerValue (port));
}

void
UdpH264ServerHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
UdpH264ServerHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;

      m_server = m_factory.Create<UdpH264Server> ();
      node->AddApplication (m_server);
      apps.Add (m_server);

    }
  return apps;
}

Ptr<UdpH264Server>
UdpH264ServerHelper::GetServer (void)
{
  return m_server;
}

UdpH264TraceClientHelper::UdpH264TraceClientHelper ()
{
}

UdpH264TraceClientHelper::UdpH264TraceClientHelper (Address address, uint16_t port, std::string filename)
{
  m_factory.SetTypeId (UdpH264TraceClient::GetTypeId ());
  SetAttribute ("RemoteAddress", AddressValue (address));
  SetAttribute ("RemotePort", UintegerValue (port));
  SetAttribute ("TraceFilename", StringValue (filename));
}

UdpH264TraceClientHelper::UdpH264TraceClientHelper (Ipv4Address address, uint16_t port, std::string filename)
{
  m_factory.SetTypeId (UdpH264TraceClient::GetTypeId ());
  SetAttribute ("RemoteAddress", AddressValue (Address (address)));
  SetAttribute ("RemotePort", UintegerValue (port));
  SetAttribute ("TraceFilename", StringValue (filename));
}

UdpH264TraceClientHelper::UdpH264TraceClientHelper (Ipv6Address address, uint16_t port, std::string filename)
{
  m_factory.SetTypeId (UdpH264TraceClient::GetTypeId ());
  SetAttribute ("RemoteAddress", AddressValue (Address (address)));
  SetAttribute ("RemotePort", UintegerValue (port));
  SetAttribute ("TraceFilename", StringValue (filename));
}

void
UdpH264TraceClientHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
UdpH264TraceClientHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<UdpH264TraceClient> client = m_factory.Create<UdpH264TraceClient> ();
      node->AddApplication (client);
      apps.Add (client);
    }
  return apps;
}

} // namespace ns3
