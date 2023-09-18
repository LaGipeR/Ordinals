//
// Created by lagiper on 08.09.23.
//

#ifndef ORDINALS_ENUMS_H
#define ORDINALS_ENUMS_H

enum OrdWalletCommand {
    CreateWallet,
    GetBalance,
    GetReceiveAddress,
    SendToAddress,
    CreateNewOrdinal,
};

enum Output {
    ToConsole,
    Silent,
    ToString,
};

enum ProgramCommand {
    UnknownCommand,
    CloseProgram,
    ShowBuffer,
    Balance,
    Address,
    Send,
    CreateOrdinal,
    UseWallet,
    CreateOrdWallet,
    Find,
};

#endif //ORDINALS_ENUMS_H
