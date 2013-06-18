/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright (c) 2007,2008, 2009 INRIA, UDcast
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
 *                              <amine.ismail@udcast.com>
 */
#include "ns3/log.h"
#include "ns3/ipv4-address.h"
#include "ns3/nstime.h"
#include "ns3/inet-socket-address.h"
#include "ns3/inet6-socket-address.h"
#include "ns3/socket.h"
#include "ns3/simulator.h"
#include "ns3/socket-factory.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"
#include "seq-ts-header.h"
//#include "udp-trace-client.h"
#include "udp-h264-trace-client.h"
#include <cstdlib>
#include <cstdio>
#include <fstream>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("UdpH264TraceClient");
NS_OBJECT_ENSURE_REGISTERED (UdpH264TraceClient);

/*
struct UdpH264TraceClient::TraceEntry UdpH264TraceClient::g_defaultEntries[] = {
  { 0, 534, 'I'},
  { 40, 1542, 'P'},
  { 120, 134, 'B'},
  { 80, 390, 'B'},
  { 240, 765, 'P'},
  { 160, 407, 'B'},
  { 200, 504, 'B'},
  { 360, 903, 'P'},
  { 280, 421, 'B'},
  { 320, 587, 'B'}
};
*/

TypeId
UdpH264TraceClient::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::UdpH264TraceClient")
    .SetParent<Application> ()
    .AddConstructor<UdpH264TraceClient> ()
    .AddAttribute ("RemoteAddress",
                   "The destination Address of the outbound packets",
                   AddressValue (),
                   MakeAddressAccessor (&UdpH264TraceClient::m_peerAddress),
                   MakeAddressChecker ())
    .AddAttribute ("RemotePort",
                   "The destination port of the outbound packets",
                   UintegerValue (100),
                   MakeUintegerAccessor (&UdpH264TraceClient::m_peerPort),
                   MakeUintegerChecker<uint16_t> ())
    .AddAttribute ("MaxPacketSize",
                   "The maximum size of a packet.",
                   UintegerValue (1024),
                   MakeUintegerAccessor (&UdpH264TraceClient::m_maxPacketSize),
                   MakeUintegerChecker<uint32_t> ())
    .AddAttribute ("TraceFilename",
                   "Name of file to load a trace from. By default, uses a hardcoded trace.",
                   StringValue (""),
                   MakeStringAccessor (&UdpH264TraceClient::SetTraceFile),
                   MakeStringChecker ())

  ;
  return tid;
}

UdpH264TraceClient::UdpH264TraceClient ()
{
  NS_LOG_FUNCTION (this);
  m_sent = 0;
  m_socket = 0;
  m_sendEvent = EventId ();
  m_maxPacketSize = 1400;
}

UdpH264TraceClient::UdpH264TraceClient (Ipv4Address ip, uint16_t port,
                                char *traceFile)
{
  NS_LOG_FUNCTION (this);
  m_sent = 0;
  m_socket = 0;
  m_sendEvent = EventId ();
  m_peerAddress = ip;
  m_peerPort = port;
  m_currentEntry = 0;
  m_maxPacketSize = 1400;
  if (traceFile != NULL)
    {
      SetTraceFile (traceFile);
    }
}

UdpH264TraceClient::~UdpH264TraceClient ()
{
  NS_LOG_FUNCTION (this);
  m_entries.clear ();
}

void
UdpH264TraceClient::SetRemote (Address ip, uint16_t port)
{
  NS_LOG_FUNCTION (this << ip << port);
  m_entries.clear ();
  m_peerAddress = ip;
  m_peerPort = port;
}

void
UdpH264TraceClient::SetRemote (Ipv4Address ip, uint16_t port)
{
  NS_LOG_FUNCTION (this << ip << port);
  m_entries.clear ();
  m_peerAddress = Address (ip);
  m_peerPort = port;
}

void
UdpH264TraceClient::SetRemote (Ipv6Address ip, uint16_t port)
{
  NS_LOG_FUNCTION (this << ip << port);
  m_entries.clear ();
  m_peerAddress = Address (ip);
  m_peerPort = port;
}

void
UdpH264TraceClient::SetTraceFile (std::string traceFile)
{
  NS_LOG_FUNCTION (this << traceFile);
  if (traceFile == "")
    {
      //NS_LOG_FUNCTION (this << "[ERROR] Trace file not specified.");
      //LoadDefaultTrace ();
    }
  else
    {
      LoadTrace (traceFile);
    }
}

void
UdpH264TraceClient::SetMaxPacketSize (uint16_t maxPacketSize)
{
  NS_LOG_FUNCTION (this << maxPacketSize);
  m_maxPacketSize = maxPacketSize;
}


uint16_t UdpH264TraceClient::GetMaxPacketSize (void)
{
  NS_LOG_FUNCTION (this);
  return m_maxPacketSize;
}


void
UdpH264TraceClient::DoDispose (void)
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void
UdpH264TraceClient::LoadTrace (std::string filename)
{
  NS_LOG_FUNCTION (this << filename);
  double txTime;
  uint16_t size;
  uint32_t lid, tid, qid, frameNo;
  TraceEntry entry;
  std::ifstream ifTraceFile;
  ifTraceFile.open (filename.c_str (), std::ifstream::in);
  m_entries.clear ();
  if (!ifTraceFile.good ())
    {
      //NS_LOG_FUNCTION (this << "[ERROR] Bad trace file: " << filename);
      //LoadDefaultTrace ();
    }
  while (ifTraceFile.good ())
    {
      ifTraceFile >> txTime >> size >> lid >> tid >> qid >> frameNo;
      NS_LOG_INFO ("Read trace entry:" << txTime << size << lid << tid << qid << frameNo);
      /*
      Input格式
         <Transmit Time>\t<Frame Size>\t<Lid>\t<Tid>\t<Qid>\t<Frame Number>

      Otput格式
         <Receive Time>\t<Frame Size>\t<Lid>\t<Tid>\t<Qid>\t<Frame Number>
      */
      /*
      if (frameType == 'B')
        {
          entry.timeToSend = 0;
        }
      else
        {
          entry.timeToSend = time - prevTime;
          prevTime = time;
        }
      */
      entry.txTime = txTime;
      entry.size = size;
      entry.lid = lid;
      entry.tid = tid;
      entry.qid = qid;
      entry.frameNo = frameNo;
      m_entries.push_back (entry);
    }
  ifTraceFile.close ();
  m_currentEntry = 0;
}

/*
void
UdpH264TraceClient::LoadDefaultTrace (void)
{
  NS_LOG_FUNCTION (this);
  uint32_t prevTime = 0;
  for (uint32_t i = 0; i < (sizeof (g_defaultEntries) / sizeof (struct TraceEntry)); i++)
    {
      struct TraceEntry entry = g_defaultEntries[i];
      if (entry.frameType == 'B')
        {
          entry.timeToSend = 0;
        }
      else
        {
          uint32_t tmp = entry.timeToSend;
          entry.timeToSend -= prevTime;
          prevTime = tmp;
        }
      m_entries.push_back (entry);
    }
  m_currentEntry = 0;
}
*/
void
UdpH264TraceClient::StartApplication (void)
{
  NS_LOG_FUNCTION (this);

  if (m_socket == 0)
    {
      TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
      m_socket = Socket::CreateSocket (GetNode (), tid);
      if (Ipv4Address::IsMatchingType(m_peerAddress) == true)
        {
          m_socket->Bind ();
          m_socket->Connect (InetSocketAddress (Ipv4Address::ConvertFrom (m_peerAddress), m_peerPort));
        }
      else if (Ipv6Address::IsMatchingType(m_peerAddress) == true)
        {
          m_socket->Bind6 ();
          m_socket->Connect (Inet6SocketAddress (Ipv6Address::ConvertFrom (m_peerAddress), m_peerPort));
        }
    }
  m_socket->SetRecvCallback (MakeNullCallback<void, Ptr<Socket> > ());
  m_sendEvent = Simulator::Schedule (Seconds (0.0), &UdpH264TraceClient::Send, this);
}

void
UdpH264TraceClient::StopApplication ()
{
  NS_LOG_FUNCTION (this);
  Simulator::Cancel (m_sendEvent);
}

void
UdpH264TraceClient::SendPacket (uint32_t size, struct TraceEntry* entry)
{
  NS_LOG_FUNCTION (this << size);
  Ptr<Packet> p;
  uint32_t packetSize;
  if (size>12)
    {
      packetSize = size - 12; // 12 is the size of the SeqTsHeader
    }
  else
    {
      packetSize = 0;
    }
  p = Create<Packet> (packetSize);//TODO: add payload here, or use header
  H264TraceHeader h264header;
  h264header.SetTraceEntry(entry)
  p->AddHeader (h264header);

  std::stringstream addressString;
  if (Ipv4Address::IsMatchingType(m_peerAddress) == true)
    {
      addressString << Ipv4Address::ConvertFrom (m_peerAddress);
    }
  else if (Ipv6Address::IsMatchingType(m_peerAddress) == true)
    {
      addressString << Ipv6Address::ConvertFrom (m_peerAddress);
    }
  else
    {
      addressString << m_peerAddress;
    }

  if ((m_socket->Send (p)) >= 0)
    {
      ++m_sent;
      NS_LOG_INFO ("Sent " << size << " bytes to "
                           << addressString.str ());
    }
  else
    {
      NS_LOG_INFO ("Error while sending " << size << " bytes to "
                                          << addressString.str ());
    }
}

void
UdpH264TraceClient::Send (void)
{
  NS_LOG_FUNCTION (this);

  NS_ASSERT (m_sendEvent.IsExpired ());
  Ptr<Packet> p;
  struct TraceEntry *entry = &m_entries[m_currentEntry];
  do
    {
      for (int i = 0; i < entry->size/ m_maxPacketSize; i++)
        {
          SendPacket (m_maxPacketSize, entry);
        }

      uint16_t sizetosend = entry->size% m_maxPacketSize;
      SendPacket (sizetosend, entry);

      m_currentEntry++;
      m_currentEntry %= m_entries.size ();
      entry = &m_entries[m_currentEntry];
    }
  while (entry->txTime == 0);
  m_sendEvent = Simulator::Schedule (MilliSeconds (entry->txTime), &UdpH264TraceClient::Send, this);
}

} // Namespace ns3
