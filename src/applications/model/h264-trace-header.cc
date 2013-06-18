/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2009 INRIA
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
 * Author: Mathieu Lacage <mathieu.lacage@sophia.inria.fr>
 */

#include "ns3/assert.h"
#include "ns3/log.h"
#include "ns3/header.h"
#include "ns3/simulator.h"
//#include "seq-ts-header.h"
#include "h264-trace-header.h"

NS_LOG_COMPONENT_DEFINE ("H264TraceHeader");

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (H264TraceHeader);

H264TraceHeader::H264TraceHeader ()
  //: m_seq (0),
    //m_ts (Simulator::Now ().GetTimeStep ())
{
  NS_LOG_FUNCTION (this);
}

/*
void
H264TraceHeader::SetTraceEntry(struct TraceEntry trace)
{
  NS_LOG_FUNCTION (this );
  m_txTime  = trace->txTime;
  m_size    = trace->size;
  m_lid     = trace->lid;
  m_tid     = trace->tid;
  m_qid     = trace->qid; 
  m_frameNo = trace->frameNo;
}
*/
uint32_t
H264TraceHeader::GetTxTime(void) const
{
  NS_LOG_FUNCTION (this);
  return m_txTime;
}
uint16_t
H264TraceHeader::GetSize(void) const
{
  NS_LOG_FUNCTION (this);
  return m_size;
}
uint32_t
H264TraceHeader::GetLid(void) const
{
  NS_LOG_FUNCTION (this);
  return m_lid;
}
uint32_t
H264TraceHeader::GetTid(void) const
{
  NS_LOG_FUNCTION (this);
  return m_tid;
}
uint32_t
H264TraceHeader::GetQid(void) const
{
  NS_LOG_FUNCTION (this);
  return m_qid;
}
uint32_t
H264TraceHeader::GetFrameNo(void) const
{
  NS_LOG_FUNCTION (this);
  return m_frameNo;
}

void
H264TraceHeader::SetTxTime(uint32_t txTime) 
{
  NS_LOG_FUNCTION (this);
  m_txTime = txTime;
}
void
H264TraceHeader::SetSize(uint16_t size) 
{
  NS_LOG_FUNCTION (this);
  m_size = size;
}
void
H264TraceHeader::SetLid(uint32_t lid) 
{
  NS_LOG_FUNCTION (this);
  m_lid = lid;
}

void
H264TraceHeader::SetTid(uint32_t tid) 
{
  NS_LOG_FUNCTION (this);
  m_tid = tid;
}
void
H264TraceHeader::SetQid(uint32_t qid) 
{
  NS_LOG_FUNCTION (this);
  m_qid = qid;
}
void
H264TraceHeader::SetFrameNo(uint32_t frameNo) 
{
  NS_LOG_FUNCTION (this);
  m_frameNo = frameNo;
}
/*
Time
H264TraceHeader::GetTs (void) const
{
  NS_LOG_FUNCTION (this);
  return TimeStep (m_ts);
}
*/

TypeId
H264TraceHeader::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::H264TraceHeader")
    .SetParent<Header> ()
    .AddConstructor<H264TraceHeader> ()
  ;
  return tid;
}
TypeId
H264TraceHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}
void
H264TraceHeader::Print (std::ostream &os) const
{
  NS_LOG_FUNCTION (this << &os);
  //os << "(seq=" << m_seq << " time=" << TimeStep (m_ts).GetSeconds () << ")";
}
uint32_t
H264TraceHeader::GetSerializedSize (void) const
{
  NS_LOG_FUNCTION (this);
  return 4+2+4+4+4+4;
}

void
H264TraceHeader::Serialize (Buffer::Iterator start) const
{
  NS_LOG_FUNCTION (this << &start);
  Buffer::Iterator i = start;
  //i.Write(m_txTime, 8);
  i.WriteHtonU32 (m_txTime);
  i.WriteHtonU16 (m_size);
  i.WriteHtonU32 (m_lid);
  i.WriteHtonU32 (m_tid);
  i.WriteHtonU32 (m_qid);
  i.WriteHtonU32 (m_frameNo);
}
uint32_t
H264TraceHeader::Deserialize (Buffer::Iterator start)
{
  NS_LOG_FUNCTION (this << &start);
  Buffer::Iterator i = start;
  //m_txTime = i.Read(8);
  m_txTime= i.ReadNtohU32 ();
  m_size = i.ReadNtohU32 ();
  m_lid = i.ReadNtohU32 ();
  m_tid = i.ReadNtohU32 ();
  m_qid = i.ReadNtohU32 (); 
  m_frameNo = i.ReadNtohU32 ();
  return GetSerializedSize ();
}

} // namespace ns3
