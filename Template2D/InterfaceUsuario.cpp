#include "InterfaceUsuario.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array<String^>^ arg) {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	PG::InterfaceUsuario form;
	Application::Run(%form);
}