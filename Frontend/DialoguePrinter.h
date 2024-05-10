#pragma once

#include "../Shared/Dialogue.h"
#include "../Backend/StateTable.h"

#ifdef _WIN32
#define DLL_EXPORT extern "C" __declspec(dllexport)
#else
#define DLL_EXPORT
#endif


namespace Moore
{
	namespace Frontend
	{
        static void (*DialoguePrinterImpl) (const Shared::Dialogue d, const Backend::StateTable st) = nullptr;
        static void (*MessagePrinterImpl) (const Shared::Dialogue d, const Backend::StateTable st) = nullptr;
        static void (*SelMessagePrinterImpl) (const std::wstring SelMsg, const Backend::StateTable st) = nullptr;


        inline void DialoguePrinter (const Shared::Dialogue d, const Backend::StateTable st)
            { return (*DialoguePrinterImpl) (d, st); }

        inline void MessagePrinter (const Shared::Dialogue d, const Backend::StateTable st)
            { return (*MessagePrinterImpl) (d, st); }

        inline void SelMessagePrinter (const std::wstring SelMsg, const Backend::StateTable st)
            { return (*SelMessagePrinterImpl) (SelMsg, st); }



        void DialoguePrinterImplemented (const Shared::Dialogue d, const Backend::StateTable st);

        void MessagePrinterImplemented (const Shared::Dialogue d, const Backend::StateTable st);

        void SelMessagePrinterImplemented (const std::wstring SelMsg, const Backend::StateTable st);

        DLL_EXPORT inline void SetImplementationPointers
        (
            void (*dialoguePrinter) (const Shared::Dialogue d, const Backend::StateTable st) = DialoguePrinterImplemented,
            void (*messagePrinter) (const Shared::Dialogue d, const Backend::StateTable st) = MessagePrinterImplemented,
            void (*selMessagePrinter) (const std::wstring SelMsg, const Backend::StateTable st) = SelMessagePrinterImplemented
        )
        {
            DialoguePrinterImpl = dialoguePrinter;
            MessagePrinterImpl = messagePrinter;
            SelMessagePrinterImpl = selMessagePrinter;
        }
	}
}

/*


        BEGIN_DIALOGUE

            TYPE=MENU
            CODE=
            TITLE=""
            DESCRIPTION=""

            BEGIN_OPTION

                CODE=
                CASH=
                POWER=
                CORRUPTION=
                TITLE=""
                SELECTION_MSG=""
                GOTO=

            END_OPTION

        END_DIALOGUE


*/