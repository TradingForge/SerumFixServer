//-------------------------------------------------------------------------------------------------
// *** f8c generated file: DO NOT EDIT! Created: 2020-01-31 01:40:33 ***
//-------------------------------------------------------------------------------------------------
/*

Fix8 is released under the GNU LESSER GENERAL PUBLIC LICENSE Version 3.

Fix8 Open Source FIX Engine.
Copyright (C) 2010-20 David L. Dight <fix@fix8.org>

Fix8 is free software: you can  redistribute it and / or modify  it under the  terms of the
GNU Lesser General  Public License as  published  by the Free  Software Foundation,  either
version 3 of the License, or (at your option) any later version.

Fix8 is distributed in the hope  that it will be useful, but WITHOUT ANY WARRANTY;  without
even the  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

You should  have received a copy of the GNU Lesser General Public  License along with Fix8.
If not, see <http://www.gnu.org/licenses/>.

*******************************************************************************************
*                Special note for Fix8 compiler generated source code                     *
*                                                                                         *
* Binary works  that are the results of compilation of code that is generated by the Fix8 *
* compiler  can be released  without releasing your  source code as  long as your  binary *
* links dynamically  against an  unmodified version of the Fix8 library.  You are however *
* required to leave the copyright text in the generated code.                             *
*                                                                                         *
*******************************************************************************************

BECAUSE THE PROGRAM IS  LICENSED FREE OF  CHARGE, THERE IS NO  WARRANTY FOR THE PROGRAM, TO
THE EXTENT  PERMITTED  BY  APPLICABLE  LAW.  EXCEPT WHEN  OTHERWISE  STATED IN  WRITING THE
COPYRIGHT HOLDERS AND/OR OTHER PARTIES  PROVIDE THE PROGRAM "AS IS" WITHOUT WARRANTY OF ANY
KIND,  EITHER EXPRESSED   OR   IMPLIED,  INCLUDING,  BUT   NOT  LIMITED   TO,  THE  IMPLIED
WARRANTIES  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  THE ENTIRE RISK AS TO
THE QUALITY AND PERFORMANCE OF THE PROGRAM IS WITH YOU. SHOULD THE PROGRAM PROVE DEFECTIVE,
YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR CORRECTION.

IN NO EVENT UNLESS REQUIRED  BY APPLICABLE LAW  OR AGREED TO IN  WRITING WILL ANY COPYRIGHT
HOLDER, OR  ANY OTHER PARTY  WHO MAY MODIFY  AND/OR REDISTRIBUTE  THE PROGRAM AS  PERMITTED
ABOVE,  BE  LIABLE  TO  YOU  FOR  DAMAGES,  INCLUDING  ANY  GENERAL, SPECIAL, INCIDENTAL OR
CONSEQUENTIAL DAMAGES ARISING OUT OF THE USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT
NOT LIMITED TO LOSS OF DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR
THIRD PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS), EVEN IF SUCH
HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

*/

//-------------------------------------------------------------------------------------------------
#include <fix8/f8config.h>
#if defined FIX8_MAGIC_NUM && FIX8_MAGIC_NUM > 16793600L
#error FIX8_LMAXTrade_types.hpp version 1.4.0 is out of date. Please regenerate with f8c.
#endif
//-------------------------------------------------------------------------------------------------
// FIX8_LMAXTrade_types.hpp
//-------------------------------------------------------------------------------------------------
#ifndef FIX8_464958385F4C4D415854726164655F74797065732E687070_
#define FIX8_464958385F4C4D415854726164655F74797065732E687070_

namespace FIX8 {
namespace LMAXTrade {

//-------------------------------------------------------------------------------------------------
using Account = Field<f8String, 1>;
//-------------------------------------------------------------------------------------------------
using AvgPx = Field<price, 6>;
//-------------------------------------------------------------------------------------------------
using BeginSeqNo = Field<SeqNum, 7>;
//-------------------------------------------------------------------------------------------------
using BeginString = Field<f8String, 8>;
//-------------------------------------------------------------------------------------------------
using BodyLength = Field<Length, 9>;
//-------------------------------------------------------------------------------------------------
using CheckSum = Field<f8String, 10>;
//-------------------------------------------------------------------------------------------------
using ClOrdID = Field<f8String, 11>;
//-------------------------------------------------------------------------------------------------
using CumQty = Field<Qty, 14>;
//-------------------------------------------------------------------------------------------------
using EndSeqNo = Field<SeqNum, 16>;
//-------------------------------------------------------------------------------------------------
using ExecID = Field<f8String, 17>;
//-------------------------------------------------------------------------------------------------
using ExecInst = Field<MultipleStringValue, 18>;
//-------------------------------------------------------------------------------------------------
using SecurityIDSource = Field<f8String, 22>;
const f8String SecurityIDSource_EXCHSYMB("8");
const size_t SecurityIDSource_realm_els(1);
//-------------------------------------------------------------------------------------------------
using LastPx = Field<price, 31>;
//-------------------------------------------------------------------------------------------------
using LastQty = Field<Qty, 32>;
//-------------------------------------------------------------------------------------------------
using MsgSeqNum = Field<SeqNum, 34>;
//-------------------------------------------------------------------------------------------------
using MsgType = Field<f8String, 35>;
const f8String MsgType_HEARTBEAT("0");
const f8String MsgType_TESTREQUEST("1");
const f8String MsgType_RESENDREQUEST("2");
const f8String MsgType_REJECT("3");
const f8String MsgType_SEQUENCERESET("4");
const f8String MsgType_LOGOUT("5");
const f8String MsgType_EXECUTIONREPORT("8");
const f8String MsgType_ORDERCANCELREJECT("9");
const f8String MsgType_LOGON("A");
const f8String MsgType_TRADECAPTUREREPORTREQUEST("AD");
const f8String MsgType_TRADECAPTUREREPORT("AE");
const f8String MsgType_TRADECAPTUREREPORTREQUESTACK("AQ");
const f8String MsgType_NEWORDERSINGLE("D");
const f8String MsgType_ORDERCANCELREQUEST("F");
const f8String MsgType_ORDERCANCELREPLACEREQUEST("G");
const f8String MsgType_ORDERSTATUSREQUEST("H");
const size_t MsgType_realm_els(16);
//-------------------------------------------------------------------------------------------------
using NewSeqNo = Field<SeqNum, 36>;
//-------------------------------------------------------------------------------------------------
using OrderID = Field<f8String, 37>;
//-------------------------------------------------------------------------------------------------
using OrderQty = Field<Qty, 38>;
//-------------------------------------------------------------------------------------------------
using OrdStatus = Field<char, 39>;
const char OrdStatus_NEW('0');
const char OrdStatus_PARTIAL('1');
const char OrdStatus_FILLED('2');
const char OrdStatus_DONE('3');
const char OrdStatus_CANCELED('4');
const char OrdStatus_PENDING_CANCEL('6');
const char OrdStatus_STOPPED('7');
const char OrdStatus_REJECTED('8');
const char OrdStatus_SUSPENDED('9');
const char OrdStatus_PENDINGNEW('A');
const char OrdStatus_CALCULATED('B');
const char OrdStatus_EXPIRED('C');
const char OrdStatus_ACCEPTBIDDING('D');
const char OrdStatus_PENDINGREP('E');
const size_t OrdStatus_realm_els(14);
//-------------------------------------------------------------------------------------------------
using OrdType = Field<char, 40>;
const char OrdType_MARKET('1');
const char OrdType_LIMIT('2');
const char OrdType_STOP('3');
const char OrdType_STOPLIMIT('4');
const size_t OrdType_realm_els(4);
//-------------------------------------------------------------------------------------------------
using OrigClOrdID = Field<f8String, 41>;
//-------------------------------------------------------------------------------------------------
using PossDupFlag = Field<Boolean, 43>;
const char PossDupFlag_ORIGTRANS('N');
const char PossDupFlag_POSSDUP('Y');
const size_t PossDupFlag_realm_els(2);
//-------------------------------------------------------------------------------------------------
using Price = Field<price, 44>;
//-------------------------------------------------------------------------------------------------
using RefSeqNum = Field<SeqNum, 45>;
//-------------------------------------------------------------------------------------------------
using SecurityID = Field<f8String, 48>;
//-------------------------------------------------------------------------------------------------
using SenderCompID = Field<f8String, 49>;
//-------------------------------------------------------------------------------------------------
using SendingTime = Field<UTCTimestamp, 52>;
//-------------------------------------------------------------------------------------------------
using Side = Field<char, 54>;
const char Side_BUY('1');
const char Side_SELL('2');
const char Side_UNDISC('7');
const size_t Side_realm_els(3);
//-------------------------------------------------------------------------------------------------
using TargetCompID = Field<f8String, 56>;
//-------------------------------------------------------------------------------------------------
using Text = Field<f8String, 58>;
//-------------------------------------------------------------------------------------------------
using TimeInForce = Field<char, 59>;
const char TimeInForce_DAY('0');
const char TimeInForce_GOODTILLCANCEL('1');
const char TimeInForce_IMMEDIATEORCANCEL('3');
const char TimeInForce_FILLORKILL('4');
const size_t TimeInForce_realm_els(4);
//-------------------------------------------------------------------------------------------------
using TransactTime = Field<UTCTimestamp, 60>;
//-------------------------------------------------------------------------------------------------
using SettlDate = Field<LocalMktDate, 64>;
//-------------------------------------------------------------------------------------------------
using TradeDate = Field<LocalMktDate, 75>;
//-------------------------------------------------------------------------------------------------
using PossResend = Field<Boolean, 97>;
const char PossResend_ORIGTRANS('N');
const char PossResend_POSSRESEND('Y');
const size_t PossResend_realm_els(2);
//-------------------------------------------------------------------------------------------------
using EncryptMethod = Field<int, 98>;
const int EncryptMethod_NONEOTHER(0);
const size_t EncryptMethod_realm_els(1);
//-------------------------------------------------------------------------------------------------
using StopPx = Field<price, 99>;
//-------------------------------------------------------------------------------------------------
using ExDestination = Field<Exchange, 100>;
//-------------------------------------------------------------------------------------------------
using CxlRejReason = Field<int, 102>;
const int CxlRejReason_TOOLATE(0);
const int CxlRejReason_UNKNOWN(1);
const int CxlRejReason_BROKEROPT(2);
const int CxlRejReason_ALREADYPENDINGCXL(3);
const int CxlRejReason_UNABLETOPROCESS(4);
const int CxlRejReason_ORIGORDMODTIMEMISMATCH(5);
const int CxlRejReason_DUPCLORDID(6);
const int CxlRejReason_OTHER(99);
const size_t CxlRejReason_realm_els(8);
//-------------------------------------------------------------------------------------------------
using OrdRejReason = Field<int, 103>;
const int OrdRejReason_BROKEROPT(0);
const int OrdRejReason_UNKNOWNSYM(1);
const int OrdRejReason_EXCHCLOSED(2);
const int OrdRejReason_EXCEEDSLIM(3);
const int OrdRejReason_TOOLATE(4);
const int OrdRejReason_UNKNOWN(5);
const int OrdRejReason_DUPLICATE(6);
const int OrdRejReason_DUPLICATEVERBAL(7);
const int OrdRejReason_STALE(8);
const int OrdRejReason_TRADEALONGREQ(9);
const int OrdRejReason_INVINVID(10);
const int OrdRejReason_UNSUPPORDERCHAR(11);
const int OrdRejReason_SURVEILLENCE(12);
const int OrdRejReason_INCORRECTQUANTITY(13);
const int OrdRejReason_INCORRECTALLOCATEDQUANTITY(14);
const int OrdRejReason_UNKNOWNACCOUNTS(15);
const int OrdRejReason_OTHER(99);
const size_t OrdRejReason_realm_els(17);
//-------------------------------------------------------------------------------------------------
using HeartBtInt = Field<int, 108>;
//-------------------------------------------------------------------------------------------------
using TestReqID = Field<f8String, 112>;
//-------------------------------------------------------------------------------------------------
using OrigSendingTime = Field<UTCTimestamp, 122>;
//-------------------------------------------------------------------------------------------------
using GapFillFlag = Field<Boolean, 123>;
const char GapFillFlag_SEQUENCERESETIGNOREMSGSEQNUMNAFORFIXMLNOTUSED('N');
const char GapFillFlag_GAPFILLMESSAGEMSGSEQNUMFIELDVALID('Y');
const size_t GapFillFlag_realm_els(2);
//-------------------------------------------------------------------------------------------------
using ResetSeqNumFlag = Field<Boolean, 141>;
const char ResetSeqNumFlag_NO('N');
const char ResetSeqNumFlag_YES('Y');
const size_t ResetSeqNumFlag_realm_els(2);
//-------------------------------------------------------------------------------------------------
using ExecType = Field<char, 150>;
const char ExecType_NEW('0');
const char ExecType_DONE('3');
const char ExecType_CANCELED('4');
const char ExecType_REPLACED('5');
const char ExecType_PENDINGCXL('6');
const char ExecType_STOPPED('7');
const char ExecType_REJECTED('8');
const char ExecType_SUSPENDED('9');
const char ExecType_PENDINGNEW('A');
const char ExecType_CALCULATED('B');
const char ExecType_EXPIRED('C');
const char ExecType_RESTATED('D');
const char ExecType_PENDINGREPLACE('E');
const char ExecType_TRADE('F');
const char ExecType_TRADECORRECT('G');
const char ExecType_TRADECANCEL('H');
const char ExecType_ORDERSTATUS('I');
const size_t ExecType_realm_els(17);
//-------------------------------------------------------------------------------------------------
using LeavesQty = Field<Qty, 151>;
//-------------------------------------------------------------------------------------------------
using SubscriptionRequestType = Field<char, 263>;
const char SubscriptionRequestType_SNAPSHOT('0');
const size_t SubscriptionRequestType_realm_els(1);
//-------------------------------------------------------------------------------------------------
using RefTagID = Field<int, 371>;
//-------------------------------------------------------------------------------------------------
using RefMsgType = Field<f8String, 372>;
//-------------------------------------------------------------------------------------------------
using SessionRejectReason = Field<int, 373>;
const int SessionRejectReason_INVALIDTAGNUMBER(0);
const int SessionRejectReason_REQUIREDTAGMISSING(1);
const int SessionRejectReason_TAGNOTDEFINEDFORTHISMESSAGETYPE(2);
const int SessionRejectReason_UNDEFINEDTAG(3);
const int SessionRejectReason_TAGSPECIFIEDWITHOUTAVALUE(4);
const int SessionRejectReason_VALUEISINCORRECTOUTOFRANGEFORTHISTAG(5);
const int SessionRejectReason_INCORRECTDATAFORMATFORVALUE(6);
const int SessionRejectReason_DECRYPTIONPROBLEM(7);
const int SessionRejectReason_SIGNATUREPROBLEM(8);
const int SessionRejectReason_COMPIDPROBLEM(9);
const int SessionRejectReason_SENDINGTIMEACCURACYPROBLEM(10);
const int SessionRejectReason_INVALIDMSGTYPE(11);
const int SessionRejectReason_XMLVALIDATIONERROR(12);
const int SessionRejectReason_TAGAPPEARSMORETHANONCE(13);
const int SessionRejectReason_TAGSPECIFIEDOUTOFREQUIREDORDER(14);
const int SessionRejectReason_REPEATINGGROUPFIELDSOUTOFORDER(15);
const int SessionRejectReason_INCORRECTNUMINGROUPCOUNTFORREPEATINGGROUP(16);
const int SessionRejectReason_NONDATAVALUEINCLUDESFIELDDELIMITERSOHCHARACTER(17);
const int SessionRejectReason_OTHER(99);
const size_t SessionRejectReason_realm_els(19);
//-------------------------------------------------------------------------------------------------
using MaxMessageSize = Field<Length, 383>;
//-------------------------------------------------------------------------------------------------
using CxlRejResponseTo = Field<char, 434>;
const char CxlRejResponseTo_ORDCXLREQ('1');
const char CxlRejResponseTo_ORDCXLREPREQ('2');
const size_t CxlRejResponseTo_realm_els(2);
//-------------------------------------------------------------------------------------------------
using SecondaryExecID = Field<f8String, 527>;
//-------------------------------------------------------------------------------------------------
using NoSides = Field<NumInGroup, 552>;
//-------------------------------------------------------------------------------------------------
using Username = Field<f8String, 553>;
//-------------------------------------------------------------------------------------------------
using Password = Field<f8String, 554>;
//-------------------------------------------------------------------------------------------------
using TradeRequestID = Field<f8String, 568>;
//-------------------------------------------------------------------------------------------------
using TradeRequestType = Field<int, 569>;
const int TradeRequestType_MATCHEDTRADES(1);
const size_t TradeRequestType_realm_els(1);
//-------------------------------------------------------------------------------------------------
using NoDates = Field<NumInGroup, 580>;
//-------------------------------------------------------------------------------------------------
using TotNumTradeReports = Field<int, 748>;
//-------------------------------------------------------------------------------------------------
using TradeRequestResult = Field<int, 749>;
const int TradeRequestResult_SUCCESSFUL(0);
const int TradeRequestResult_OTHER(99);
const size_t TradeRequestResult_realm_els(2);
//-------------------------------------------------------------------------------------------------
using TradeRequestStatus = Field<int, 750>;
const int TradeRequestStatus_ACCEPTED(0);
const int TradeRequestStatus_REJECTED(2);
const size_t TradeRequestStatus_realm_els(2);
//-------------------------------------------------------------------------------------------------
using OrdStatusReqID = Field<f8String, 790>;
//-------------------------------------------------------------------------------------------------
using LastRptRequested = Field<Boolean, 912>;
//-------------------------------------------------------------------------------------------------
using MatchIncrement = Field<Qty, 1089>;
//-------------------------------------------------------------------------------------------------
using FIX8_LMAXTrade_BaseEntry = FieldTable;

} // namespace LMAXTrade
} // namespace FIX8
#endif // FIX8_464958385F4C4D415854726164655F74797065732E687070_
