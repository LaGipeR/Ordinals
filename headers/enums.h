//
// Created by lagiper on 08.09.23.
//

#ifndef ORDINALS_ENUMS_H
#define ORDINALS_ENUMS_H

enum OrdWalletCommand {
    CreateNewWallet,
    GetBalance,
    GetReceiveAddress,
    SendToAddress,
    CreateNewOrdinal,
};

enum Output {
    ToBuffer,
    ToConsole,
    ToBufferAndConsole,
};

enum ProgramCommand {
    UnknownCommand,
    CloseProgram,
    RequestToBitcoinNode,
    RequestToOrdWallet,
    ShowBuffer,
    Balance,
    Address,
    Send,
    Create,
};

#endif //ORDINALS_ENUMS_H
