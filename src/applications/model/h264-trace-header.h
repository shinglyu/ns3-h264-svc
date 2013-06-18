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

#ifndef H264_TRACE_HEADER_H
#define H264_TRACE_HEADER_H

#include "ns3/header.h"
#include "ns3/nstime.h"

namespace ns3 {
/**
 * \ingroup udpclientserver
 * \class H264TraceHeader
 * \brief Packet header for Udp client/server application
 * The header is made of a 32bits sequence number followed by
 * a 64bits time stamp.
 */
/*
struct TraceEntry
{
  double txTime;
  uint16_t size;
  uint32_t lid;
  uint32_t tid;
  uint32_t qid; 
  uint32_t frameNo;
};
*/

class H264TraceHeader : public Header
{
public:
  H264TraceHeader ();

  /**
   * \param seq the sequence number
   */
  //void SetTraceEntry (struct TraceEntry trace);
  /**
   * \return the sequence number
   */
  //TraceEntry GetTraceEntry (void) const;
  double GetTxTime(void) const;
  uint16_t GetSize(void) const;
  uint32_t GetLid(void) const;
  uint32_t GetTid(void) const;
  uint32_t GetQid(void) const;
  uint32_t GetFrameNo(void) const;
  double SetTxTime(double) const;
  uint16_t SetSize(uint16_t) const;
  uint32_t SetLid(uint32_t) const;
  uint32_t SetTid(uint32_t) const;
  uint32_t SetQid(uint32_t) const;
  uint32_t SetFrameNo(uint32_t) const;

  static TypeId GetTypeId (void);
private:
  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream &os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator start) const;
  virtual uint32_t Deserialize (Buffer::Iterator start);

  double m_txTime;
  uint16_t m_size;
  uint32_t m_lid;
  uint32_t m_tid;
  uint32_t m_qid; 
  uint32_t m_frameNo;
};

} // namespace ns3

#endif /* H264_TRACE_HEADER_H */
