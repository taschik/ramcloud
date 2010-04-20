/* Copyright (c) 2010 Stanford University
 *
 * Permission to use, copy, modify, and distribute this software for any purpose
 * with or without fee is hereby granted, provided that the above copyright
 * notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR(S) DISCLAIM ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL AUTHORS BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
 * CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/**
 * \file RPC.cc Implementation of the ClientRPC and ServerRPC classes.
 */

#include <Buffer.h>
#include <RPC.h>

namespace RAMCloud {

void ClientRPC::startRPC(Service *dest, Buffer* rpcPayload) {
    // Send the RPC. Hang onto the buffer in case we need to retransmit.

    if (dest->getServiceId() == 0) {
        token.s = NULL;
        return;
    }

    transport()->clientSend(dest, rpcPayload, &token);
    this->rpcPayload = rpcPayload;
}

Buffer* ClientRPC::getReply() {
    // Check if replyPayload is set. Call blocking recv if not.

    if (token.s == NULL) return NULL;

    if (!replyPayload) transport()->clientRecv(replyPayload, &token);
    return replyPayload;
}

Buffer* ServerRPC::getRequest() {
    // Block on serverRecv;
    transport()->serverRecv(reqPayload, &token);
    return reqPayload;
}

void ServerRPC::sendReply(Buffer* replyPayload) {
    // Send the RPC. Don't hang onto the buffer, put it in the history list of
    // replies.
    transport()->serverSend(replyPayload, &token);
}

}  // namespace RAMCloud
