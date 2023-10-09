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

enum ProgramCommand {
    CloseProgram,
    Balance,
    Address,
    Send,
    CreateOrdinal,
    UseWallet,
    CreateOrdWallet,
    Find,
    Help,
    Ordinals
};

#endif //ORDINALS_ENUMS_H
