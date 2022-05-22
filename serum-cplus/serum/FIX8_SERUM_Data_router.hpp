//-------------------------------------------------------------------------------------------------
// *** f8c generated file: DO NOT EDIT! Created: 2022-05-15 19:24:33 ***
//-------------------------------------------------------------------------------------------------
/*

Fix8 is released under the GNU LESSER GENERAL PUBLIC LICENSE Version 3.

Fix8 Open Source FIX Engine.
Copyright (C) 2010-22 David L. Dight <fix@fix8.org>

Fix8 is free software: yfou can  redistribute it and / or modify  it under the  terms of the
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
#error FIX8_SERUM_Data_router.hpp version 1.4.1 is out of date. Please regenerate with f8c.
#endif
//-------------------------------------------------------------------------------------------------
// FIX8_SERUM_Data_router.hpp
//-------------------------------------------------------------------------------------------------
#ifndef FIX8_464958385F534552554D5F446174615F726F757465722E687070_
#define FIX8_464958385F534552554D5F446174615F726F757465722E687070_

namespace FIX8 {
namespace SERUM_Data {

//-------------------------------------------------------------------------------------------------
class FIX8_SERUM_Data_Router : public Router
{
public:
   FIX8_SERUM_Data_Router() {}
   virtual ~FIX8_SERUM_Data_Router() {}

   virtual bool operator() (const class Message *msg) const { return false; }
   virtual bool operator() (const class Heartbeat *msg) const { return true; }
   virtual bool operator() (const class TestRequest *msg) const { return true; }
   virtual bool operator() (const class ResendRequest *msg) const { return true; }
   virtual bool operator() (const class Reject *msg) const { return true; }
   virtual bool operator() (const class SequenceReset *msg) const { return true; }
   virtual bool operator() (const class Logout *msg) const { return true; }
   virtual bool operator() (const class IOI *msg) const { return false; }
   virtual bool operator() (const class Advertisement *msg) const { return false; }
   virtual bool operator() (const class ExecutionReport *msg) const { return false; }
   virtual bool operator() (const class OrderCancelReject *msg) const { return false; }
   virtual bool operator() (const class Logon *msg) const { return true; }
   virtual bool operator() (const class DerivativeSecurityList *msg) const { return false; }
   virtual bool operator() (const class NewOrderMultileg *msg) const { return false; }
   virtual bool operator() (const class MultilegOrderCancelReplace *msg) const { return false; }
   virtual bool operator() (const class TradeCaptureReportRequest *msg) const { return false; }
   virtual bool operator() (const class TradeCaptureReport *msg) const { return false; }
   virtual bool operator() (const class OrderMassStatusRequest *msg) const { return false; }
   virtual bool operator() (const class QuoteRequestReject *msg) const { return false; }
   virtual bool operator() (const class RFQRequest *msg) const { return false; }
   virtual bool operator() (const class QuoteStatusReport *msg) const { return false; }
   virtual bool operator() (const class QuoteResponse *msg) const { return false; }
   virtual bool operator() (const class Confirmation *msg) const { return false; }
   virtual bool operator() (const class PositionMaintenanceRequest *msg) const { return false; }
   virtual bool operator() (const class PositionMaintenanceReport *msg) const { return false; }
   virtual bool operator() (const class RequestForPositions *msg) const { return false; }
   virtual bool operator() (const class RequestForPositionsAck *msg) const { return false; }
   virtual bool operator() (const class PositionReport *msg) const { return false; }
   virtual bool operator() (const class TradeCaptureReportRequestAck *msg) const { return false; }
   virtual bool operator() (const class TradeCaptureReportAck *msg) const { return false; }
   virtual bool operator() (const class AllocationReport *msg) const { return false; }
   virtual bool operator() (const class AllocationReportAck *msg) const { return false; }
   virtual bool operator() (const class ConfirmationAck *msg) const { return false; }
   virtual bool operator() (const class SettlementInstructionRequest *msg) const { return false; }
   virtual bool operator() (const class AssignmentReport *msg) const { return false; }
   virtual bool operator() (const class CollateralRequest *msg) const { return false; }
   virtual bool operator() (const class CollateralAssignment *msg) const { return false; }
   virtual bool operator() (const class CollateralResponse *msg) const { return false; }
   virtual bool operator() (const class News *msg) const { return false; }
   virtual bool operator() (const class CollateralReport *msg) const { return false; }
   virtual bool operator() (const class CollateralInquiry *msg) const { return false; }
   virtual bool operator() (const class NetworkCounterpartySystemStatusRequest *msg) const { return false; }
   virtual bool operator() (const class NetworkCounterpartySystemStatusResponse *msg) const { return false; }
   virtual bool operator() (const class UserRequest *msg) const { return false; }
   virtual bool operator() (const class UserResponse *msg) const { return false; }
   virtual bool operator() (const class CollateralInquiryAck *msg) const { return false; }
   virtual bool operator() (const class ConfirmationRequest *msg) const { return false; }
   virtual bool operator() (const class Email *msg) const { return false; }
   virtual bool operator() (const class NewOrderSingle *msg) const { return false; }
   virtual bool operator() (const class NewOrderList *msg) const { return false; }
   virtual bool operator() (const class OrderCancelRequest *msg) const { return false; }
   virtual bool operator() (const class OrderCancelReplaceRequest *msg) const { return false; }
   virtual bool operator() (const class OrderStatusRequest *msg) const { return false; }
   virtual bool operator() (const class AllocationInstruction *msg) const { return false; }
   virtual bool operator() (const class ListCancelRequest *msg) const { return false; }
   virtual bool operator() (const class ListExecute *msg) const { return false; }
   virtual bool operator() (const class ListStatusRequest *msg) const { return false; }
   virtual bool operator() (const class ListStatus *msg) const { return false; }
   virtual bool operator() (const class AllocationInstructionAck *msg) const { return false; }
   virtual bool operator() (const class DontKnowTrade *msg) const { return false; }
   virtual bool operator() (const class QuoteRequest *msg) const { return false; }
   virtual bool operator() (const class Quote *msg) const { return false; }
   virtual bool operator() (const class SettlementInstructions *msg) const { return false; }
   virtual bool operator() (const class MarketDataRequest *msg) const { return false; }
   virtual bool operator() (const class MarketDataSnapshotFullRefresh *msg) const { return false; }
   virtual bool operator() (const class MarketDataIncrementalRefresh *msg) const { return false; }
   virtual bool operator() (const class MarketDataRequestReject *msg) const { return false; }
   virtual bool operator() (const class QuoteCancel *msg) const { return false; }
   virtual bool operator() (const class QuoteStatusRequest *msg) const { return false; }
   virtual bool operator() (const class MassQuoteAcknowledgement *msg) const { return false; }
   virtual bool operator() (const class SecurityDefinitionRequest *msg) const { return false; }
   virtual bool operator() (const class SecurityDefinition *msg) const { return false; }
   virtual bool operator() (const class SecurityStatusRequest *msg) const { return false; }
   virtual bool operator() (const class SecurityStatus *msg) const { return false; }
   virtual bool operator() (const class TradingSessionStatusRequest *msg) const { return false; }
   virtual bool operator() (const class TradingSessionStatus *msg) const { return false; }
   virtual bool operator() (const class MassQuote *msg) const { return false; }
   virtual bool operator() (const class BusinessMessageReject *msg) const { return false; }
   virtual bool operator() (const class BidRequest *msg) const { return false; }
   virtual bool operator() (const class BidResponse *msg) const { return false; }
   virtual bool operator() (const class ListStrikePrice *msg) const { return false; }
   virtual bool operator() (const class RegistrationInstructions *msg) const { return false; }
   virtual bool operator() (const class RegistrationInstructionsResponse *msg) const { return false; }
   virtual bool operator() (const class OrderMassCancelRequest *msg) const { return false; }
   virtual bool operator() (const class OrderMassCancelReport *msg) const { return false; }
   virtual bool operator() (const class NewOrderCross *msg) const { return false; }
   virtual bool operator() (const class CrossOrderCancelReplaceRequest *msg) const { return false; }
   virtual bool operator() (const class CrossOrderCancelRequest *msg) const { return false; }
   virtual bool operator() (const class SecurityTypeRequest *msg) const { return false; }
   virtual bool operator() (const class SecurityTypes *msg) const { return false; }
   virtual bool operator() (const class SecurityListRequest *msg) const { return false; }
   virtual bool operator() (const class SecurityList *msg) const { return false; }
   virtual bool operator() (const class DerivativeSecurityListRequest *msg) const { return false; }
};

} // namespace SERUM_Data
} // namespace FIX8
#endif // FIX8_464958385F534552554D5F446174615F726F757465722E687070_
