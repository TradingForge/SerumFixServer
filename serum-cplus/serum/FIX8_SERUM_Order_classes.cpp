//-------------------------------------------------------------------------------------------------
// *** f8c generated file: DO NOT EDIT! Created: 2023-02-26 15:05:29 ***
//-------------------------------------------------------------------------------------------------
/*

Fix8 is released under the GNU LESSER GENERAL PUBLIC LICENSE Version 3.

Fix8 Open Source FIX Engine.
Copyright (C) 2010-23 David L. Dight <fix@fix8.org>

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
#if defined FIX8_MAGIC_NUM && FIX8_MAGIC_NUM > 16793601L
#error FIX8_SERUM_Order_classes.cpp version 1.4.1 is out of date. Please regenerate with f8c.
#endif
//-------------------------------------------------------------------------------------------------
// FIX8_SERUM_Order_classes.cpp
//-------------------------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <iterator>
#include <algorithm>
#include <cerrno>
#include <string.h>
// f8 includes
#include <fix8/f8exception.hpp>
#include <fix8/hypersleep.hpp>
#include <fix8/mpmc.hpp>
#include <fix8/thread.hpp>
#include <fix8/f8types.hpp>
#include <fix8/f8utils.hpp>
#include <fix8/tickval.hpp>
#include <fix8/logger.hpp>
#include <fix8/traits.hpp>
#include <fix8/field.hpp>
#include <fix8/message.hpp>
#include "FIX8_SERUM_Order_types.hpp"
#include "FIX8_SERUM_Order_router.hpp"
#include "FIX8_SERUM_Order_classes.hpp"
//-------------------------------------------------------------------------------------------------
namespace FIX8 {
namespace SERUM_Order {

namespace {

//-------------------------------------------------------------------------------------------------

const char *cn[] // Component names
{
   "",
   "AffectedOrdGrp", // 1
   "AllocAckGrp", // 2
   "AllocGrp", // 3
   "AttrbGrp", // 4
   "BidCompReqGrp", // 5
   "BidCompRspGrp", // 6
   "BidDescReqGrp", // 7
   "ClrInstGrp", // 8
   "CollInqQualGrp", // 9
   "CommissionData", // 10
   "CompIDReqGrp", // 11
   "CompIDStatGrp", // 12
   "ContAmtGrp", // 13
   "ContraGrp", // 14
   "CpctyConfGrp", // 15
   "DiscretionInstructions", // 16
   "DlvyInstGrp", // 17
   "EvntGrp", // 18
   "ExecAllocGrp", // 19
   "ExecCollGrp", // 20
   "ExecsGrp", // 21
   "FinancingDetails", // 22
   "IOIQualGrp", // 23
   "InstrmtGrp", // 24
   "InstrmtLegExecGrp", // 25
   "InstrmtLegGrp", // 26
   "InstrmtLegIOIGrp", // 27
   "InstrmtLegSecListGrp", // 28
   "InstrmtMDReqGrp", // 29
   "InstrmtStrkPxGrp", // 30
   "Instrument", // 31
   "InstrumentExtension", // 32
   "InstrumentLeg", // 33
   "LegBenchmarkCurveData", // 34
   "LegOrdGrp", // 35
   "LegPreAllocGrp", // 36
   "LegQuotGrp", // 37
   "LegQuotStatGrp", // 38
   "LegSecAltIDGrp", // 39
   "LegStipulations", // 40
   "LinesOfTextGrp", // 41
   "ListOrdGrp", // 42
   "MDFullGrp", // 43
   "MDIncGrp", // 44
   "MDReqGrp", // 45
   "MDRjctGrp", // 46
   "MiscFeesGrp", // 47
   "NestedParties", // 48
   "NestedParties2", // 49
   "NestedParties3", // 50
   "NstdPtys2SubGrp", // 51
   "NstdPtys3SubGrp", // 52
   "NstdPtysSubGrp", // 53
   "OrdAllocGrp", // 54
   "OrdListStatGrp", // 55
   "OrderQtyData", // 56
   "Parties", // 57
   "PegInstructions", // 58
   "PosUndInstrmtGrp", // 59
   "PositionAmountData", // 60
   "PositionQty", // 61
   "PreAllocGrp", // 62
   "PreAllocMlegGrp", // 63
   "PtysSubGrp", // 64
   "QuotCxlEntriesGrp", // 65
   "QuotEntryAckGrp", // 66
   "QuotEntryGrp", // 67
   "QuotQualGrp", // 68
   "QuotReqGrp", // 69
   "QuotReqLegsGrp", // 70
   "QuotReqRjctGrp", // 71
   "QuotSetAckGrp", // 72
   "QuotSetGrp", // 73
   "RFQReqGrp", // 74
   "RelSymDerivSecGrp", // 75
   "RgstDistInstGrp", // 76
   "RgstDtlsGrp", // 77
   "RoutingGrp", // 78
   "SecAltIDGrp", // 79
   "SecListGrp", // 80
   "SecTypesGrp", // 81
   "SettlInstGrp", // 82
   "SettlInstructionsData", // 83
   "SettlParties", // 84
   "SettlPtysSubGrp", // 85
   "SideCrossOrdCxlGrp", // 86
   "SideCrossOrdModGrp", // 87
   "SpreadOrBenchmarkCurveData", // 88
   "Stipulations", // 89
   "TrdAllocGrp", // 90
   "TrdCapDtGrp", // 91
   "TrdCapRptSideGrp", // 92
   "TrdCollGrp", // 93
   "TrdInstrmtLegGrp", // 94
   "TrdRegTimestamps", // 95
   "TrdgSesGrp", // 96
   "UndInstrmtCollGrp", // 97
   "UndInstrmtGrp", // 98
   "UndInstrmtStrkPxGrp", // 99
   "UndSecAltIDGrp", // 100
   "UnderlyingInstrument", // 101
   "UnderlyingStipulations", // 102
   "YieldData", // 103
};

} // namespace

//-------------------------------------------------------------------------------------------------
const SERUM_Order::FIX8_SERUM_Order_BaseMsgEntry::Pair msgpairs[] 
{
   { "0", { Type2Type<SERUM_Order::Heartbeat>(), "Heartbeat" } },
   { "1", { Type2Type<SERUM_Order::TestRequest>(), "TestRequest" } },
   { "2", { Type2Type<SERUM_Order::ResendRequest>(), "ResendRequest" } },
   { "3", { Type2Type<SERUM_Order::Reject>(), "Reject" } },
   { "4", { Type2Type<SERUM_Order::SequenceReset>(), "SequenceReset" } },
   { "5", { Type2Type<SERUM_Order::Logout>(), "Logout" } },
   { "6", { Type2Type<SERUM_Order::IOI>(), "IOI" } },
   { "7", { Type2Type<SERUM_Order::Advertisement>(), "Advertisement" } },
   { "8", { Type2Type<SERUM_Order::ExecutionReport>(), "ExecutionReport" } },
   { "9", { Type2Type<SERUM_Order::OrderCancelReject>(), "OrderCancelReject" } },
   { "A", { Type2Type<SERUM_Order::Logon>(), "Logon" } },
   { "AA", { Type2Type<SERUM_Order::DerivativeSecurityList>(), "DerivativeSecurityList" } },
   { "AB", { Type2Type<SERUM_Order::NewOrderMultileg>(), "NewOrderMultileg" } },
   { "AC", { Type2Type<SERUM_Order::MultilegOrderCancelReplace>(), "MultilegOrderCancelReplace" } },
   { "AD", { Type2Type<SERUM_Order::TradeCaptureReportRequest>(), "TradeCaptureReportRequest" } },
   { "AE", { Type2Type<SERUM_Order::TradeCaptureReport>(), "TradeCaptureReport" } },
   { "AF", { Type2Type<SERUM_Order::OrderMassStatusRequest>(), "OrderMassStatusRequest" } },
   { "AG", { Type2Type<SERUM_Order::QuoteRequestReject>(), "QuoteRequestReject" } },
   { "AH", { Type2Type<SERUM_Order::RFQRequest>(), "RFQRequest" } },
   { "AI", { Type2Type<SERUM_Order::QuoteStatusReport>(), "QuoteStatusReport" } },
   { "AJ", { Type2Type<SERUM_Order::QuoteResponse>(), "QuoteResponse" } },
   { "AK", { Type2Type<SERUM_Order::Confirmation>(), "Confirmation" } },
   { "AL", { Type2Type<SERUM_Order::PositionMaintenanceRequest>(), "PositionMaintenanceRequest" } },
   { "AM", { Type2Type<SERUM_Order::PositionMaintenanceReport>(), "PositionMaintenanceReport" } },
   { "AN", { Type2Type<SERUM_Order::RequestForPositions>(), "RequestForPositions" } },
   { "AO", { Type2Type<SERUM_Order::RequestForPositionsAck>(), "RequestForPositionsAck" } },
   { "AP", { Type2Type<SERUM_Order::PositionReport>(), "PositionReport" } },
   { "AQ", { Type2Type<SERUM_Order::TradeCaptureReportRequestAck>(), "TradeCaptureReportRequestAck" } },
   { "AR", { Type2Type<SERUM_Order::TradeCaptureReportAck>(), "TradeCaptureReportAck" } },
   { "AS", { Type2Type<SERUM_Order::AllocationReport>(), "AllocationReport" } },
   { "AT", { Type2Type<SERUM_Order::AllocationReportAck>(), "AllocationReportAck" } },
   { "AU", { Type2Type<SERUM_Order::ConfirmationAck>(), "ConfirmationAck" } },
   { "AV", { Type2Type<SERUM_Order::SettlementInstructionRequest>(), "SettlementInstructionRequest" } },
   { "AW", { Type2Type<SERUM_Order::AssignmentReport>(), "AssignmentReport" } },
   { "AX", { Type2Type<SERUM_Order::CollateralRequest>(), "CollateralRequest" } },
   { "AY", { Type2Type<SERUM_Order::CollateralAssignment>(), "CollateralAssignment" } },
   { "AZ", { Type2Type<SERUM_Order::CollateralResponse>(), "CollateralResponse" } },
   { "B", { Type2Type<SERUM_Order::News>(), "News" } },
   { "BA", { Type2Type<SERUM_Order::CollateralReport>(), "CollateralReport" } },
   { "BB", { Type2Type<SERUM_Order::CollateralInquiry>(), "CollateralInquiry" } },
   { "BC", { Type2Type<SERUM_Order::NetworkCounterpartySystemStatusRequest>(), "NetworkCounterpartySystemStatusRequest" } },
   { "BD", { Type2Type<SERUM_Order::NetworkCounterpartySystemStatusResponse>(), "NetworkCounterpartySystemStatusResponse" } },
   { "BE", { Type2Type<SERUM_Order::UserRequest>(), "UserRequest" } },
   { "BF", { Type2Type<SERUM_Order::UserResponse>(), "UserResponse" } },
   { "BG", { Type2Type<SERUM_Order::CollateralInquiryAck>(), "CollateralInquiryAck" } },
   { "BH", { Type2Type<SERUM_Order::ConfirmationRequest>(), "ConfirmationRequest" } },
   { "C", { Type2Type<SERUM_Order::Email>(), "Email" } },
   { "D", { Type2Type<SERUM_Order::NewOrderSingle>(), "NewOrderSingle" } },
   { "E", { Type2Type<SERUM_Order::NewOrderList>(), "NewOrderList" } },
   { "F", { Type2Type<SERUM_Order::OrderCancelRequest>(), "OrderCancelRequest" } },
   { "G", { Type2Type<SERUM_Order::OrderCancelReplaceRequest>(), "OrderCancelReplaceRequest" } },
   { "H", { Type2Type<SERUM_Order::OrderStatusRequest>(), "OrderStatusRequest" } },
   { "J", { Type2Type<SERUM_Order::AllocationInstruction>(), "AllocationInstruction" } },
   { "K", { Type2Type<SERUM_Order::ListCancelRequest>(), "ListCancelRequest" } },
   { "L", { Type2Type<SERUM_Order::ListExecute>(), "ListExecute" } },
   { "M", { Type2Type<SERUM_Order::ListStatusRequest>(), "ListStatusRequest" } },
   { "N", { Type2Type<SERUM_Order::ListStatus>(), "ListStatus" } },
   { "P", { Type2Type<SERUM_Order::AllocationInstructionAck>(), "AllocationInstructionAck" } },
   { "Q", { Type2Type<SERUM_Order::DontKnowTrade>(), "DontKnowTrade" } },
   { "R", { Type2Type<SERUM_Order::QuoteRequest>(), "QuoteRequest" } },
   { "S", { Type2Type<SERUM_Order::Quote>(), "Quote" } },
   { "T", { Type2Type<SERUM_Order::SettlementInstructions>(), "SettlementInstructions" } },
   { "V", { Type2Type<SERUM_Order::MarketDataRequest>(), "MarketDataRequest" } },
   { "W", { Type2Type<SERUM_Order::MarketDataSnapshotFullRefresh>(), "MarketDataSnapshotFullRefresh" } },
   { "X", { Type2Type<SERUM_Order::MarketDataIncrementalRefresh>(), "MarketDataIncrementalRefresh" } },
   { "Y", { Type2Type<SERUM_Order::MarketDataRequestReject>(), "MarketDataRequestReject" } },
   { "Z", { Type2Type<SERUM_Order::QuoteCancel>(), "QuoteCancel" } },
   { "a", { Type2Type<SERUM_Order::QuoteStatusRequest>(), "QuoteStatusRequest" } },
   { "b", { Type2Type<SERUM_Order::MassQuoteAcknowledgement>(), "MassQuoteAcknowledgement" } },
   { "c", { Type2Type<SERUM_Order::SecurityDefinitionRequest>(), "SecurityDefinitionRequest" } },
   { "d", { Type2Type<SERUM_Order::SecurityDefinition>(), "SecurityDefinition" } },
   { "e", { Type2Type<SERUM_Order::SecurityStatusRequest>(), "SecurityStatusRequest" } },
   { "f", { Type2Type<SERUM_Order::SecurityStatus>(), "SecurityStatus" } },
   { "g", { Type2Type<SERUM_Order::TradingSessionStatusRequest>(), "TradingSessionStatusRequest" } },
   { "h", { Type2Type<SERUM_Order::TradingSessionStatus>(), "TradingSessionStatus" } },
   { "header", { Type2Type<SERUM_Order::header, bool>(), "header" } },
   { "i", { Type2Type<SERUM_Order::MassQuote>(), "MassQuote" } },
   { "j", { Type2Type<SERUM_Order::BusinessMessageReject>(), "BusinessMessageReject" } },
   { "k", { Type2Type<SERUM_Order::BidRequest>(), "BidRequest" } },
   { "l", { Type2Type<SERUM_Order::BidResponse>(), "BidResponse" } },
   { "m", { Type2Type<SERUM_Order::ListStrikePrice>(), "ListStrikePrice" } },
   { "n", { Type2Type<SERUM_Order::XMLnonFIX>(), "XMLnonFIX" } },
   { "o", { Type2Type<SERUM_Order::RegistrationInstructions>(), "RegistrationInstructions" } },
   { "p", { Type2Type<SERUM_Order::RegistrationInstructionsResponse>(), "RegistrationInstructionsResponse" } },
   { "q", { Type2Type<SERUM_Order::OrderMassCancelRequest>(), "OrderMassCancelRequest" } },
   { "r", { Type2Type<SERUM_Order::OrderMassCancelReport>(), "OrderMassCancelReport" } },
   { "s", { Type2Type<SERUM_Order::NewOrderCross>(), "NewOrderCross" } },
   { "t", { Type2Type<SERUM_Order::CrossOrderCancelReplaceRequest>(), "CrossOrderCancelReplaceRequest" } },
   { "trailer", { Type2Type<SERUM_Order::trailer, bool>(), "trailer" } },
   { "u", { Type2Type<SERUM_Order::CrossOrderCancelRequest>(), "CrossOrderCancelRequest" } },
   { "v", { Type2Type<SERUM_Order::SecurityTypeRequest>(), "SecurityTypeRequest" } },
   { "w", { Type2Type<SERUM_Order::SecurityTypes>(), "SecurityTypes" } },
   { "x", { Type2Type<SERUM_Order::SecurityListRequest>(), "SecurityListRequest" } },
   { "y", { Type2Type<SERUM_Order::SecurityList>(), "SecurityList" } },
   { "z", { Type2Type<SERUM_Order::DerivativeSecurityListRequest>(), "DerivativeSecurityListRequest" } }
}; // 95

extern const FIX8_SERUM_Order_BaseEntry::Pair fldpairs[];

/// Compiler generated metadata object, accessed through this function.
const F8MetaCntx& ctx() // avoid SIOF
{
   static const FIX8_SERUM_Order_BaseMsgEntry bme(msgpairs, 95);
   static const FIX8_SERUM_Order_BaseEntry be(fldpairs, 912);
   static const F8MetaCntx _ctx(4400, bme, be, cn, "FIX.4.4");
   return _ctx;
}

} // namespace SERUM_Order

// Compiler generated metadata object accessible outside namespace through this function.
extern "C"
{
   const F8MetaCntx& SERUM_Order_ctx() { return SERUM_Order::ctx(); }
}

} // namespace FIX8

