#include <Windows.h>
#include "resource.h"
#include <CommDlg.h>
#include <fstream>
using namespace std;
struct Usuario
{
    char nombreus[15];
    char nombrecompleto[50];
    char contraus[10];
    char usuariotipo[20];
    char comercious[15];
    char fotous[300];
    char direccionus[100];
    char primera[5];
    Usuario* Usuario_sig;
    Usuario* Usuario_ant;
};
Usuario* Usuario_aux, * Usuario_inicio, * NUsuario, * miInfo = nullptr;
char zFile[MAX_PATH];
char direccion[MAX_PATH] = { 0 };


struct Cliente
{
	char nombrecliente[50];
	char clienteemail[25];
	char comerciocliente[15];
	char fechacliente[50];
	Cliente* Cliente_sig;
	Cliente* Cliente_ant;

};
Cliente* Cliente_inicio, * Cliente_aux = nullptr;


struct Consumo
{
	char Concliente[50];
	char Conmonto[50];
	char Consubtotal[50];
	char Condescuento[50];
	char Contotal[50];
	char Concomercio[15];
	char Confecha[50];
	char ConID[50];
	Consumo* Consumo_sig;
	Consumo* Consumo_ant;
};
Consumo* Consumo_inicio, * Consumo_aux = nullptr;

struct Promocion
{
	char Promnombre[50];
	char Prommonto[50];
	char Promdescuento[50];
	char Promestatus[15];
	char Promcomercio[15];
	char Promfecha[50];
	Promocion* Promocion_sig;
	Promocion* Promocion_ant;
};
Promocion* Promocion_inicio, * Promocion_aux = nullptr;

HMENU hmenu;
HINSTANCE hintance;
HWND ghDlg = 0;
HINSTANCE hInstGlobal;	
HWND hPrinGlob;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Registro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PantallaPrincipal(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK infosuario(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK USUARIOALTA(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK listausuarios(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK usuariocontra(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK clientealta(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK actualizarcliente(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK altaconsumo(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK listaconsumo(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK filtro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK altapromocion(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK listapromo(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);



void Agregar_usuario(Usuario* nuevo_u);
void Leer_Binario_Usuario();
void Escribir_Binario_Usuario();
void Eliminar_Usuario(char nomUsuario[50]);


void Agregar_Cliente(Cliente* nuevo);
void Escribir_Binario_Cliente();
void Leer_Binario_Cliente();
void Eliminar_Cliente(char nomCliente[50]);


void Agregar_Consumo(Consumo* nuevo);
void Escribir_Binario_Consumo();
void Leer_Binario_Consumo();

void Agregar_Promocion(Promocion* nuevo);
void Escribir_Binario_Promocion();
void Leer_Binario_Promocion();
void Eliminar_Promocion(char nomPromocion[50]);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	Leer_Binario_Usuario();
	Leer_Binario_Cliente();
	Leer_Binario_Consumo();
	Leer_Binario_Promocion();
	hInstGlobal = hInst;
	HWND hVInicio = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, WindowProc);

	ShowWindow(hVInicio, cmdshow);

	MSG Mensaje;
	ZeroMemory(&Mensaje, sizeof(Mensaje));

	while (GetMessage(&Mensaje, 0, 0, 0)) {
		TranslateMessage(&Mensaje);
		DispatchMessage(&Mensaje);
	}
	Escribir_Binario_Usuario(); //escribe el binario al terminar el programa
	Escribir_Binario_Cliente();
	Escribir_Binario_Promocion();
	Escribir_Binario_Consumo();
	return Mensaje.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {

    case WM_INITDIALOG:
        break;

    case WM_COMMAND:

        switch (LOWORD(wParam))
        {
        case IDC_BUTTON1:
		{ //Validacion de inicio de sesion
			char Usuario_nombre[15];
			char Usuario_contrasena[10];
			char Usuario_comercio[15];
			GetDlgItemText(hwnd, 2, Usuario_nombre, sizeof(Usuario_nombre));
			GetDlgItemText(hwnd, 3, Usuario_contrasena, sizeof(Usuario_contrasena));
			GetDlgItemText(hwnd, 1, Usuario_comercio, sizeof(Usuario_comercio));

			Usuario_aux = Usuario_inicio;
			if (Usuario_inicio == nullptr) {
				MessageBox(hwnd, "No hay usuario registrados.", "AVISO", MB_OK | MB_ICONERROR);
			}
			else {
				while (Usuario_aux != nullptr && strcmp(Usuario_aux->nombreus, Usuario_nombre) != 0) {
					Usuario_aux = Usuario_aux->Usuario_sig;
				}
				if (Usuario_aux == nullptr) {
					MessageBox(hwnd, "Usuario no encontrado", "AVISO", MB_OK | MB_ICONERROR);
				}
				else {
					if (strcmp(Usuario_aux->contraus, Usuario_contrasena) == 0)
					{
						if (strcmp(Usuario_aux->comercious, Usuario_comercio) == 0) {
							NUsuario = Usuario_aux;
							miInfo = Usuario_aux;
							MessageBox(hwnd, "Ya puede ingresar", "AVISO", MB_OK | MB_ICONEXCLAMATION);

							if (strcmp(Usuario_aux->primera, "0") == 0) {//validacion de si ya ingreso antes
								EndDialog(hwnd, 0);
							
								DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG4), hwnd, infosuario);	
								DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, PantallaPrincipal);

							}
							else {
								EndDialog(hwnd, 0);
								DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, PantallaPrincipal);
							}
						}
						else {
							MessageBox(hwnd, "Comercio incorrecto", "AVISO", MB_OK | MB_ICONERROR);
						}
					}
					else {
						MessageBox(hwnd, "Contraseña incorrecta", "AVISO", MB_OK | MB_ICONERROR);
					}
				}
			}
		}break;
            break;
        case IDC_BUTTON2:
               DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG2), NULL, Registro);
            break;
        case IDC_BUTTON3:
			Escribir_Binario_Usuario();
			Escribir_Binario_Cliente();
			Escribir_Binario_Promocion();
			Escribir_Binario_Consumo();
            PostQuitMessage(0);
            break;
        }
        break;

    }

    return FALSE;
}
LRESULT CALLBACK Registro(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (uMsg)
    {

	case WM_INITDIALOG: { //combo box Tipo de usuario
		char TipodeUsuario[2][14] = { "Administrador", "Cajero" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, 7, CB_INSERTSTRING, (WPARAM)i, (LPARAM)TipodeUsuario[i]);
		}
		SendMessage(GetDlgItem(hwnd, 7), CB_SETCURSEL, (WPARAM)0, 0);
	}

        break;


    case WM_COMMAND:

        switch (LOWORD(wParam))
        {
     
            case IDC_BUTTON5:
            {


                int numletras = 0;
                int numnumeros = 0;
                int numcarac = 0;
                int numespacio = 0;
                int tamanombre = 0;
                int tamacomer = 0;
                int tamacontra = 0;
				Usuario* Nuevo_usuario = new Usuario;
				GetDlgItemText(hwnd, 5, Nuevo_usuario->nombreus, sizeof(Nuevo_usuario->nombreus));
				GetDlgItemText(hwnd, 6, Nuevo_usuario->contraus, sizeof(Nuevo_usuario->contraus));
				GetDlgItemText(hwnd, 4, Nuevo_usuario->comercious, sizeof(Nuevo_usuario->comercious));

				int UsuarioSelec = 0;
				UsuarioSelec = SendMessage(GetDlgItem(hwnd, 7), CB_GETCURSEL, 0, 0);
				GetDlgItemText(hwnd,7, Nuevo_usuario->usuariotipo, sizeof(Nuevo_usuario->usuariotipo));
				//Nuevo_usuario->Usuario_tipo = UsuarioSelec + 1;
				tamacomer = strlen(Nuevo_usuario->comercious);
				tamanombre = strlen(Nuevo_usuario->nombreus);
				tamacontra = strlen(Nuevo_usuario->contraus);

				char comparar[15];
				GetDlgItemText(hwnd, 1008, comparar, sizeof(comparar));
				Usuario_aux = Usuario_inicio;



				//VALIDACION
				if (tamanombre <= 0 || tamanombre > 15)//valida nombre
				{
					MessageBox(hwnd, "Favor de ingresar de un maximo de 15 caracteres alfabeticos (Sin espacios).", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
				}
				else {
					for (int i = 0; i < tamanombre; i++)
					{
						if (ispunct(Nuevo_usuario->nombreus[i]))
						{
							numcarac++;
						}
						if (isdigit(Nuevo_usuario->nombreus[i]))
						{
							numnumeros++;
						}
						if (isspace(Nuevo_usuario->nombreus[i]))
						{
							numespacio++;
						}
					}
					if (numcarac != 0 || numnumeros != 0 || numespacio != 0)
					{
						MessageBox(hwnd, "Favor de solo ingresar caracteres alfabeticos (Sin espacios).", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
					}
					else
					{
						numletras = 0;
						numnumeros = 0;
						numcarac = 0;
						numespacio = 0;
						while (Usuario_aux != nullptr && strcmp(Usuario_aux->nombreus, comparar) != 0) {//validacion mismmo nombre
							Usuario_aux = Usuario_aux->Usuario_sig;
						}
						if (Usuario_aux != nullptr) {
							MessageBox(hwnd, "Usuario ya existente, intente otro nombre", "AVISO", MB_OK | MB_ICONERROR);
						}
						else {
							if (tamacontra< 3 || tamacontra>10)//valida contraseña
							{
								MessageBox(hwnd, "Favor de ingresar de 3 a 10 caracteres. Minimo 1 letra, 1 numero y 1 caracter especial (Sin espacios).", "CONTRASEÑA", MB_OK | MB_ICONEXCLAMATION);
							}
							else {
								for (int i = 0; i < tamacontra; i++)
								{
									if (isalpha(Nuevo_usuario->contraus[i]))
									{
										numletras++;
									}
									if (ispunct(Nuevo_usuario->contraus[i]))
									{
										numcarac++;
									}
									if (isdigit(Nuevo_usuario->contraus[i]))
									{
										numnumeros++;
									}
									if (isspace(Nuevo_usuario->contraus[i]))
									{
										numespacio++;
									}
								}
								if (numcarac == 0 || numnumeros == 0 ||numletras == 0 || numespacio != 0)
								{
									MessageBox(hwnd, "Favor de ingresar minimo 1 letra, 1 numero y 1 caracter especial(Sin espacios).", "CONTRASEÑA", MB_OK | MB_ICONEXCLAMATION);
								}
								else
								{
									numletras = 0;
									numnumeros = 0;
									numcarac = 0;
									numespacio = 0;
									if (tamacomer < 5 || tamacomer>15)//valida comercio
									{
										MessageBox(hwnd, "Favor de ingresar de 5 a 15 caracteres alfanumericos o espacios", "COMERCIO", MB_OK | MB_ICONEXCLAMATION);
									}
									else
									{
										for (int i = 0; i <tamacomer; i++)
										{
											if (ispunct(Nuevo_usuario->comercious[i]))
											{
												numcarac++;
											}
										}
										if (numcarac != 0)
										{
											MessageBox(hwnd, "Favor de solo ingresar caracteres alfanumericos o espacios", "COMERCIO", MB_OK | MB_ICONEXCLAMATION);
										}
										else
										{//Registro exitoso
											//MessageBox(hwnd, "Usuario registrado", "AVISO", MB_OK | MB_ICONEXCLAMATION);
											strcpy_s(Nuevo_usuario->fotous, "");
											strcpy_s(Nuevo_usuario->usuariotipo, "Administrador");
											strcpy_s(Nuevo_usuario->nombrecompleto, "");
											strcpy_s(Nuevo_usuario->direccionus, "");
											strcpy_s(Nuevo_usuario->primera, "0");
											Agregar_usuario(Nuevo_usuario);
											delete Nuevo_usuario;
											Escribir_Binario_Usuario();

											SetDlgItemText(hwnd, 5, "");
											SetDlgItemText(hwnd, 6, "");
											SetDlgItemText(hwnd, 4, "");
										}
									}
								}
							}
						}

					}
				}
              

            }
            break;
        case IDC_BUTTON3:
        {
            DestroyWindow(hwnd);
        }
            break;
        }
        break;



    }

    return FALSE;
}
LRESULT CALLBACK PantallaPrincipal(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:HMENU hmenu;
	case WM_INITDIALOG: {//rellena lista de usuarios
		{hmenu = LoadMenu(hintance, MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(hwnd, hmenu); }
		char Usuario_comercio_c[15];
		int CClientes = 0;
		char TotalClientes[100] = { 0 };
		char ClientesTotal[100] = { 0 };

		int CPA = 0;
		char PromoAct[100] = { 0 };
		char Act[100] = { 0 };

		int CPI = 0;
		char PromoIn[100] = { 0 };
		char In[100] = { 0 };

		float CST = 0;
		float Total = 0;
		char ConsumoTotal[100] = { 0 };
		char TotalC[100] = { 0 };

		strcpy_s(Usuario_comercio_c, NUsuario->comercious);

		Cliente_aux = Cliente_inicio;
		while (Cliente_aux != nullptr) {
			if (strcmp(Usuario_comercio_c, Cliente_aux->comerciocliente) == 0) {
				SendMessage(GetDlgItem(hwnd, 137), LB_ADDSTRING, 0, (LPARAM)Cliente_aux->nombrecliente);
				CClientes++;
			}
			Cliente_aux = Cliente_aux->Cliente_sig;
		}
		sprintf_s(TotalClientes, "%d", CClientes);
		strcat_s(ClientesTotal, TotalClientes);
		SetDlgItemText(hwnd, 131, ClientesTotal);

		Promocion_aux = Promocion_inicio;
		while (Promocion_aux != nullptr) {
			if (strcmp(Usuario_comercio_c, Promocion_aux->Promcomercio) == 0) {
				if (strcmp(Promocion_aux->Promestatus, "Activa") == 0)
				{
					CPA++;
				}
				if (strcmp(Promocion_aux->Promestatus, "Inactiva") == 0)
				{
					CPI++;
				}
			}
			Promocion_aux = Promocion_aux->Promocion_sig;
		}
		sprintf_s(PromoAct, "%d", CPA);
		strcat_s(Act, PromoAct);
		SetDlgItemText(hwnd, 132, Act);

		sprintf_s(PromoIn, "%d", CPI);
		strcat_s(In, PromoIn);
		SetDlgItemText(hwnd, 133, In);

		Usuario_aux = Usuario_inicio;

		if (strcmp(NUsuario->usuariotipo, "Cajero") == 0)
		{
			EnableMenuItem(GetMenu(hwnd), 996, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), 999, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), 993, MF_DISABLED | MF_BYCOMMAND);

		}

		Consumo_aux = Consumo_inicio;
		while (Consumo_aux != nullptr) {
			if (strcmp(Usuario_comercio_c, Consumo_aux->Concomercio) == 0) {
				Total = atof(Consumo_aux->Contotal);
				CST = CST + Total;
			}
			Consumo_aux = Consumo_aux->Consumo_sig;
		}
		sprintf_s(ConsumoTotal, "%.2f", CST);
		strcat_s(TotalC, ConsumoTotal);
		SetDlgItemText(hwnd, 136, TotalC);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->fotous, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 134, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 135, NUsuario->nombreus);

	}break;

	case WM_COMMAND: {
		char textoList[50] = { 0 };
		if (LOWORD(wParam) == 137) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				char Usuario_comercio_c[15];
				strcpy_s(Usuario_comercio_c, NUsuario->comercious);
				int indice = 0;
				float MM = 0;
				float TT = 0;
				char Mostrar[50] = { 0 };
				char SumaCliente[50] = { 0 };

				indice = SendDlgItemMessage(hwnd, 137, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, 137, LB_GETTEXT, indice, (LPARAM)textoList);


				Consumo_aux = Consumo_inicio;
				while (Consumo_aux != nullptr) {
					if (strcmp(Usuario_comercio_c, Consumo_aux->Concomercio) == 0) {
						if (strcmp(textoList, Consumo_aux->Concliente) == 0) {

							MM = atof(Consumo_aux->Contotal);
							TT = TT + MM;

						}
					}
					Consumo_aux = Consumo_aux->Consumo_sig;
				}
				sprintf_s(Mostrar, "%.2f", TT);
				strcat_s(SumaCliente, Mostrar);
				SetDlgItemText(hwnd, 138, SumaCliente);



			}break;
			}
		}
		switch (LOWORD(wParam))
		{
		case 999:
		{
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG5), hwnd, USUARIOALTA);
		}break;
		case 998:
		{
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG6), hwnd, listausuarios);

		}break;
		case 997:
		{				EndDialog(hwnd, 0);

			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG4), hwnd, infosuario);
		}break;
		case 996:
		{
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG7), hwnd, usuariocontra);
		}break;
		case 995:
		{
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG8), hwnd, clientealta);
		}break;
		case 994:
		{
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG9), hwnd, actualizarcliente);
		}break;
		case 993:
		{
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG13), hwnd, altapromocion);
		}break;
		case 992:
		{
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG14), hwnd, listapromo);
		}break;
		case 991:
		{
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG10), hwnd, altaconsumo);
		}break;
		case 990:
		{
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG11), hwnd, listaconsumo);
		}break;
		case 989:
		{
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG12), hwnd, filtro);
		}break;
		case 139:
		{Escribir_Binario_Usuario();
		Escribir_Binario_Cliente();
		Escribir_Binario_Promocion();
		Escribir_Binario_Consumo();
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, WindowProc);
		}break;
		case 988:
		{
			int Opcion = MessageBox(0, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (Opcion) {
			case IDYES: {
				Escribir_Binario_Usuario();
				Escribir_Binario_Cliente();
				Escribir_Binario_Promocion();
				Escribir_Binario_Consumo();
				EndDialog(hwnd, 0);
				DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, WindowProc);
			}break;
			}break;
		}

		}break;

	}
	}
	return false;
	
}
LRESULT CALLBACK infosuario(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{

	case WM_INITDIALOG:
	{
		if (strcmp(NUsuario->usuariotipo, "Cajero") == 0)
		{
			EnableMenuItem(GetMenu(hwnd), 996, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), 999, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), 993, MF_DISABLED | MF_BYCOMMAND);
		}
		SetDlgItemText(hwnd, 8, NUsuario->nombreus);
		SetDlgItemText(hwnd, 9, NUsuario->nombrecompleto);
		SetDlgItemText(hwnd, 10, NUsuario->comercious);
		SetDlgItemText(hwnd, 11, NUsuario->direccionus);
		strcpy_s(zFile, NUsuario->fotous);


		SendDlgItemMessage(hwnd, 13, CB_SELECTSTRING, 0, (LPARAM)NUsuario->usuariotipo);
		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->fotous, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 15, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

		char TipodeUsuario[2][14] = { "Administrador", "Cajero" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, 13, CB_INSERTSTRING, (WPARAM)i, (LPARAM)TipodeUsuario[i]);
		}
		SendMessage(GetDlgItem(hwnd, 13), CB_SELECTSTRING, 0, (LPARAM)NUsuario->usuariotipo);
		/*SendMessage(GetDlgItem(hwnd, 1024), CB_SETCURSEL, (WPARAM)NUsuario->Usuario_tipo - 1, 0);*/
		if (strcmp(NUsuario->primera, "0") == 0) {//si es la primera vez todas las ventanas estan activas
			EnableWindow(GetDlgItem(hwnd, 1022), true);
			EnableWindow(GetDlgItem(hwnd, 1027), true);
			EnableWindow(GetDlgItem(hwnd, 1020), true);

			EnableMenuItem(GetMenu(hwnd), 997, MF_DISABLED | MF_BYCOMMAND);

			EnableMenuItem(GetMenu(hwnd), 999, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), 998, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), 996, MF_DISABLED | MF_BYCOMMAND);

			EnableMenuItem(GetMenu(hwnd), 995, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), 994, MF_DISABLED | MF_BYCOMMAND);

			EnableMenuItem(GetMenu(hwnd), 993, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), 992, MF_DISABLED | MF_BYCOMMAND);

			EnableMenuItem(GetMenu(hwnd), 991, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), 990, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), 989, MF_DISABLED | MF_BYCOMMAND);
		}
	}
	break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 16:
		{
			EnableWindow(GetDlgItem(hwnd, 10), true);
			EnableWindow(GetDlgItem(hwnd, 8), true);
			EnableWindow(GetDlgItem(hwnd, 9), true);
			EnableWindow(GetDlgItem(hwnd, 12), true);
			break;
		case 14: {//Registrar usuario	
			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthNombre = 0;
			int lengthComercio = 0;
			int lengthContrasena = 0;
			GetDlgItemText(hwnd, 9, NUsuario->nombrecompleto, sizeof(NUsuario->nombrecompleto));
			GetDlgItemText(hwnd, 11, NUsuario->direccionus, sizeof(NUsuario->direccionus));


			lengthNombre = strlen(NUsuario->nombrecompleto);

			int UsuarioSelec = 0;
			UsuarioSelec = SendMessage(GetDlgItem(hwnd, 13), CB_GETCURSEL, 0, 0);
			strcpy_s(NUsuario->fotous, zFile);
			if (strcmp(NUsuario->nombrecompleto, "") == 0) {//validacion si ya escribiio nombre
				MessageBox(hwnd, "Favor de escribir su nombre completo", "AVISO", MB_OK | MB_ICONERROR);
				strcpy_s(NUsuario->primera, "0");
				EnableMenuItem(GetMenu(hwnd), 997, MF_DISABLED | MF_BYCOMMAND);

				EnableMenuItem(GetMenu(hwnd), 999, MF_DISABLED | MF_BYCOMMAND);
				EnableMenuItem(GetMenu(hwnd), 998, MF_DISABLED | MF_BYCOMMAND);
				EnableMenuItem(GetMenu(hwnd), 996, MF_DISABLED | MF_BYCOMMAND);

				EnableMenuItem(GetMenu(hwnd), 995, MF_DISABLED | MF_BYCOMMAND);
				EnableMenuItem(GetMenu(hwnd), 994, MF_DISABLED | MF_BYCOMMAND);

				EnableMenuItem(GetMenu(hwnd), 993, MF_DISABLED | MF_BYCOMMAND);
				EnableMenuItem(GetMenu(hwnd), 992, MF_DISABLED | MF_BYCOMMAND);

				EnableMenuItem(GetMenu(hwnd), 991, MF_DISABLED | MF_BYCOMMAND);
				EnableMenuItem(GetMenu(hwnd), 990, MF_DISABLED | MF_BYCOMMAND);
				EnableMenuItem(GetMenu(hwnd), 989, MF_DISABLED | MF_BYCOMMAND);

			}
			else {
				if (lengthNombre < 5 || lengthNombre > 50)
				{
					MessageBox(hwnd, "Favor de ingresar de 5 a 50 caracteres alfabeticos.", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
					strcpy_s(NUsuario->primera, "0");
					EnableMenuItem(GetMenu(hwnd),997, MF_DISABLED | MF_BYCOMMAND);

					EnableMenuItem(GetMenu(hwnd), 999, MF_DISABLED | MF_BYCOMMAND);
					EnableMenuItem(GetMenu(hwnd), 998, MF_DISABLED | MF_BYCOMMAND);
					EnableMenuItem(GetMenu(hwnd), 996, MF_DISABLED | MF_BYCOMMAND);

					EnableMenuItem(GetMenu(hwnd), 995, MF_DISABLED | MF_BYCOMMAND);
					EnableMenuItem(GetMenu(hwnd), 994, MF_DISABLED | MF_BYCOMMAND);

					EnableMenuItem(GetMenu(hwnd), 993, MF_DISABLED | MF_BYCOMMAND);
					EnableMenuItem(GetMenu(hwnd), 992, MF_DISABLED | MF_BYCOMMAND);

					EnableMenuItem(GetMenu(hwnd), 991, MF_DISABLED | MF_BYCOMMAND);
					EnableMenuItem(GetMenu(hwnd), 990, MF_DISABLED | MF_BYCOMMAND);
					EnableMenuItem(GetMenu(hwnd), 989, MF_DISABLED | MF_BYCOMMAND);
				}
				else {
					for (int i = 0; i < lengthNombre; i++)
					{
						if (ispunct(NUsuario->nombrecompleto[i]))
						{
							CCaracteres++;
						}
						if (isdigit(NUsuario->nombrecompleto[i]))
						{
							CNumeros++;
						}
					}
					if (CCaracteres != 0 || CNumeros != 0)
					{
						MessageBox(hwnd, "Favor de solo ingresar caracteres alfabeticos.", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
						strcpy_s(NUsuario->primera, "0");
						EnableMenuItem(GetMenu(hwnd), 997, MF_DISABLED | MF_BYCOMMAND);

						EnableMenuItem(GetMenu(hwnd), 999, MF_DISABLED | MF_BYCOMMAND);
						EnableMenuItem(GetMenu(hwnd), 998, MF_DISABLED | MF_BYCOMMAND);
						EnableMenuItem(GetMenu(hwnd), 996, MF_DISABLED | MF_BYCOMMAND);

						EnableMenuItem(GetMenu(hwnd), 995, MF_DISABLED | MF_BYCOMMAND);
						EnableMenuItem(GetMenu(hwnd), 994, MF_DISABLED | MF_BYCOMMAND);

						EnableMenuItem(GetMenu(hwnd), 993, MF_DISABLED | MF_BYCOMMAND);
						EnableMenuItem(GetMenu(hwnd), 992, MF_DISABLED | MF_BYCOMMAND);

						EnableMenuItem(GetMenu(hwnd), 991, MF_DISABLED | MF_BYCOMMAND);
						EnableMenuItem(GetMenu(hwnd), 990, MF_DISABLED | MF_BYCOMMAND);
						EnableMenuItem(GetMenu(hwnd), 989, MF_DISABLED | MF_BYCOMMAND);
					}
					else
					{
						if (strcmp(NUsuario->direccionus, "") == 0) {//validacion si ya eligio foto
							MessageBox(hwnd, "Favor de seleccionar una foto", "AVISO", MB_OK | MB_ICONERROR);
							strcpy_s(NUsuario->primera, "0");
							EnableMenuItem(GetMenu(hwnd), 997, MF_DISABLED | MF_BYCOMMAND);

							EnableMenuItem(GetMenu(hwnd), 999, MF_DISABLED | MF_BYCOMMAND);
							EnableMenuItem(GetMenu(hwnd), 998, MF_DISABLED | MF_BYCOMMAND);
							EnableMenuItem(GetMenu(hwnd), 996, MF_DISABLED | MF_BYCOMMAND);

							EnableMenuItem(GetMenu(hwnd), 995, MF_DISABLED | MF_BYCOMMAND);
							EnableMenuItem(GetMenu(hwnd), 994, MF_DISABLED | MF_BYCOMMAND);

							EnableMenuItem(GetMenu(hwnd), 993, MF_DISABLED | MF_BYCOMMAND);
							EnableMenuItem(GetMenu(hwnd), 992, MF_DISABLED | MF_BYCOMMAND);

							EnableMenuItem(GetMenu(hwnd), 991, MF_DISABLED | MF_BYCOMMAND);
							EnableMenuItem(GetMenu(hwnd), 990, MF_DISABLED | MF_BYCOMMAND);
							EnableMenuItem(GetMenu(hwnd), 989, MF_DISABLED | MF_BYCOMMAND);
						}
						else {
							strcpy_s(NUsuario->primera, "1");
						
							EnableWindow(GetDlgItem(hwnd, 8), false);
							EnableWindow(GetDlgItem(hwnd, 10), false);
							EnableWindow(GetDlgItem(hwnd, 9), false);
							EnableWindow(GetDlgItem(hwnd, 12), false);

							EnableMenuItem(GetMenu(hwnd), 997, MF_ENABLED | MF_BYCOMMAND);

							EnableMenuItem(GetMenu(hwnd), 999, MF_ENABLED | MF_BYCOMMAND);
							EnableMenuItem(GetMenu(hwnd), 998, MF_ENABLED | MF_BYCOMMAND);
							EnableMenuItem(GetMenu(hwnd), 996, MF_ENABLED | MF_BYCOMMAND);

							EnableMenuItem(GetMenu(hwnd), 995, MF_ENABLED | MF_BYCOMMAND);
							EnableMenuItem(GetMenu(hwnd), 994, MF_ENABLED | MF_BYCOMMAND);

							EnableMenuItem(GetMenu(hwnd), 993, MF_ENABLED | MF_BYCOMMAND);
							EnableMenuItem(GetMenu(hwnd), 992, MF_ENABLED | MF_BYCOMMAND);

							EnableMenuItem(GetMenu(hwnd), 991, MF_ENABLED | MF_BYCOMMAND);
							EnableMenuItem(GetMenu(hwnd), 990, MF_ENABLED | MF_BYCOMMAND);
							EnableMenuItem(GetMenu(hwnd), 989, MF_ENABLED | MF_BYCOMMAND);
							Escribir_Binario_Usuario();
							if (strcmp(NUsuario->usuariotipo, "Cajero") == 0)
							{
								EnableMenuItem(GetMenu(hwnd), 996, MF_DISABLED | MF_BYCOMMAND);
								EnableMenuItem(GetMenu(hwnd), 999, MF_DISABLED | MF_BYCOMMAND);
								EnableMenuItem(GetMenu(hwnd), 993, MF_DISABLED | MF_BYCOMMAND);

							}
						}
					}
				}
			}
		}break;
		case 12: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP bmp; //1

				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE); //2
				SetDlgItemText(hwnd, 11, zFile);
				SendDlgItemMessage(hwnd,15, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); //3 
			}
			else {
				MessageBox(hwnd, "No seleccionó foto", "AVISO", MB_OK | MB_ICONERROR);
				strcpy_s(zFile, NUsuario->fotous);

			}
		}break;
		case 17: {//Regresar a menu
			if (strcmp(NUsuario->primera, "0") == 0) {//validacion de si ya ingreso antes
				MessageBox(hwnd, "Favor de registrar los datos", "AVISO", MB_OK | MB_ICONERROR);
			}
			else {
				Escribir_Binario_Usuario();
				EndDialog(hwnd,0 );
		
				DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, PantallaPrincipal);
				
			}
		}break;

		case 988: {//Salir			
			int Opcion = MessageBox(0, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (Opcion) {
			case IDYES: {
				DestroyWindow(hwnd);
			}
					  break;
			}
		}break;
		case 999:
		{
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG5), hwnd, USUARIOALTA);
		}break;
		}
		}
	}break;
	}
	return FALSE;
}
LRESULT CALLBACK USUARIOALTA(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_INITDIALOG: { //combo box Tipo de usuario
		if (strcmp(NUsuario->usuariotipo, "Cajero") == 0)
		{
			EnableMenuItem(GetMenu(hwnd), 996, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), 999, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), 993, MF_DISABLED | MF_BYCOMMAND);

		}
		SetDlgItemText(hwnd, 17, NUsuario->comercious);
		SendDlgItemMessage(hwnd, 21, CB_SELECTSTRING, 0, (LPARAM)NUsuario->usuariotipo);

		char TipodeUsuario[2][14] = { "Administrador", "Cajero" };
		for (int i = 0; i < 2; i++) {
			SendDlgItemMessage(hwnd, 21, CB_INSERTSTRING, (WPARAM)i, (LPARAM)TipodeUsuario[i]);
		}
		SendMessage(GetDlgItem(hwnd,21), CB_SELECTSTRING, 0, (LPARAM)NUsuario->usuariotipo);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->fotous, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 26, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 777, NUsuario->nombreus);
		/*SendMessage(GetDlgItem(hwnd, 1037), CB_SETCURSEL, (WPARAM)0, 0);*/
	}break;

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {

		case 25: {//Regresar a menu
			Escribir_Binario_Usuario();
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, PantallaPrincipal);
		}
				 break;
		case 24: { //Boton de Aceptar 
			int numletras = 0;
			int numnumeros = 0;
			int numcarac = 0;
			int numespacio = 0;
			int tamanombre = 0;
			int tamacomercio = 0;
			int tamacontra = 0;


			Usuario* Nuevo_usuario = new Usuario;
			GetDlgItemText(hwnd, 18, Nuevo_usuario->nombreus, sizeof(Nuevo_usuario->nombreus));
			GetDlgItemText(hwnd, 19, Nuevo_usuario->contraus, sizeof(Nuevo_usuario->contraus));
			GetDlgItemText(hwnd, 17, Nuevo_usuario->comercious, sizeof(Nuevo_usuario->comercious));
			//checar lo de tipo de usuario
			int UsuarioSelec = 0;
			UsuarioSelec = SendMessage(GetDlgItem(hwnd, 21), CB_GETCURSEL, 0, 0);
			/*GetDlgItemText(hwnd, 1024, Nuevo_usuario->Usuario_tipo, sizeof(Nuevo_usuario->Usuario_tipo));*/
			GetDlgItemText(hwnd, 21, Nuevo_usuario->usuariotipo, sizeof(Nuevo_usuario->usuariotipo));
			/*Nuevo_usuario->Usuario_tipo = UsuarioSelec + 1;*/
			tamanombre = strlen(Nuevo_usuario->nombreus);
			tamacontra = strlen(Nuevo_usuario->contraus);
			char Usuario_nombre_registro[15];
			GetDlgItemText(hwnd, 18, Usuario_nombre_registro, sizeof(Usuario_nombre_registro));
			Usuario_aux = Usuario_inicio;

			if (tamanombre <= 0 || tamanombre > 15)//valida nombre
			{
				MessageBox(hwnd, "Favor de ingresar de un maximo de 15 caracteres alfabeticos (Sin espacios).", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
			}
			else {
				for (int i = 0; i < tamanombre; i++)
				{
					if (ispunct(Nuevo_usuario->nombreus[i]))
					{
						numcarac++;
					}
					if (isdigit(Nuevo_usuario->nombreus[i]))
					{
						numnumeros++;
					}
					if (isspace(Nuevo_usuario->nombreus[i]))
					{
						numespacio++;
					}
				}
				if (numcarac != 0 || numnumeros != 0 || numespacio != 0)
				{
					MessageBox(hwnd, "Favor de solo ingresar caracteres alfabeticos (Sin espacios).", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
				}
				else
				{
					numletras= 0;
					numnumeros = 0;
					numcarac = 0;
					numespacio = 0;
					while (Usuario_aux != nullptr && strcmp(Usuario_aux->nombreus, Usuario_nombre_registro) != 0) {//validacion mismmo nombre
						Usuario_aux = Usuario_aux->Usuario_sig;
					}
					if (Usuario_aux != nullptr) {
						MessageBox(hwnd, "Usuario ya existente, intente otro nombre", "AVISO", MB_OK | MB_ICONERROR);
					}
					else {
						if (tamacontra < 3 || tamacontra>10)//valida contraseña
						{
							MessageBox(hwnd, "Favor de ingresar de 3 a 10 caracteres. Minimo 1 letra, 1 numero y 1 caracter especial (Sin espacios).", "CONTRASEÑA", MB_OK | MB_ICONEXCLAMATION);
						}
						else {
							for (int i = 0; i < tamacontra; i++)
							{
								if (isalpha(Nuevo_usuario->contraus[i]))
								{
									numletras++;
								}
								if (ispunct(Nuevo_usuario->contraus[i]))
								{
									numcarac++;
								}
								if (isdigit(Nuevo_usuario->contraus[i]))
								{
									numnumeros++;
								}
								if (isspace(Nuevo_usuario->contraus[i]))
								{
									numespacio++;
								}
							}
							if (numcarac == 0 || numnumeros == 0 || numletras == 0 || numespacio != 0)
							{
								MessageBox(hwnd, "Favor de ingresar minimo 1 letra, 1 numero y 1 caracter especial(Sin espacios).", "CONTRASEÑA", MB_OK | MB_ICONEXCLAMATION);
							}
							else
							{//registrro exitoso
								strcpy_s(Nuevo_usuario->fotous, "");
								strcpy_s(Nuevo_usuario->nombrecompleto, "");
								strcpy_s(Nuevo_usuario->direccionus, "");
								strcpy_s(Nuevo_usuario->primera, "0");
								Agregar_usuario(Nuevo_usuario);
								delete Nuevo_usuario;
								Escribir_Binario_Usuario();

								SetDlgItemText(hwnd, 18, "");
								SetDlgItemText(hwnd, 19, "");
							}
						}
					}

				}
			}
		}break;


		case 988: {//Salir			
			int Opcion = MessageBox(0, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (Opcion) {
			case IDYES: {
				DestroyWindow(hwnd);
			}
					  break;
			}
		}break;

		}
	}break;
	}
	return false;
}
LRESULT CALLBACK listausuarios(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char textoList[50] = { 0 };
	switch (msg) 
	{
	
	case WM_INITDIALOG: 
	{
		{
		}//rellena lista de usuarios
		if (strcmp(NUsuario->usuariotipo, "Cajero") == 0)
		{
			EnableMenuItem(GetMenu(hwnd), 996, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), 999, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), 993, MF_DISABLED | MF_BYCOMMAND);

		}
		char Usuario_comercio_c[15];
		strcpy_s(Usuario_comercio_c, NUsuario->comercious);

		char TipodeUsuario[2][14] = { "Administrador", "Cajero" };
		for (int i = 0; i < 2; i++)
		{
			SendDlgItemMessage(hwnd, 40, CB_INSERTSTRING, (WPARAM)i, (LPARAM)TipodeUsuario[i]);
		}


		Usuario_aux = Usuario_inicio;
		while (Usuario_aux != nullptr) {
			if (strcmp(Usuario_comercio_c, Usuario_aux->comercious) == 0) {
				SendMessage(GetDlgItem(hwnd, 41), LB_ADDSTRING, 0, (LPARAM)Usuario_aux->nombreus);
			}
			Usuario_aux = Usuario_aux->Usuario_sig;
		}

		if (strcmp(NUsuario->usuariotipo, "Cajero") == 0)
		{
			EnableWindow(GetDlgItem(hwnd, 43), false);
			EnableWindow(GetDlgItem(hwnd, 44), false);
			EnableWindow(GetDlgItem(hwnd, 50), false);


		}
		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->fotous, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 47, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 778, NUsuario->nombreus);

	}break;
	
	case WM_COMMAND: 
	{
		if (LOWORD(wParam) == 41)
		{

			switch (HIWORD(wParam))
			{
			case LBN_DBLCLK:
			{

				int indice = 0;

				indice = SendDlgItemMessage(hwnd, 41, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, 41, LB_GETTEXT, indice, (LPARAM)textoList);

				Usuario_aux = Usuario_inicio;
				while (Usuario_aux != nullptr && strcmp(textoList, Usuario_aux->nombreus) != 0) {
					Usuario_aux = Usuario_aux->Usuario_sig;
				}
				if (Usuario_aux == nullptr) {

				}
				else
				{
					SetDlgItemText(hwnd, 43, Usuario_aux->nombreus);
					SetDlgItemText(hwnd, 44, Usuario_aux->nombrecompleto);
					SetDlgItemText(hwnd, 42, Usuario_aux->comercious);
					SetDlgItemText(hwnd, 45, Usuario_aux->direccionus);
					SendDlgItemMessage(hwnd, 40, CB_SELECTSTRING, 0, (LPARAM)Usuario_aux->usuariotipo);
					HBITMAP bmp;
					bmp = (HBITMAP)LoadImage(NULL, Usuario_aux->fotous, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
					SendDlgItemMessage(hwnd, 46, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				}

			}break;
			}
		}
		if (LOWORD(wParam) == 53) {
			EndDialog(hwnd, 0);
			
		}
		if (LOWORD(wParam) == 999) {
			//case  40019: {//De Pantallap principal a USUARIOS ALTA
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG5), hwnd, USUARIOALTA);
		}
		if (LOWORD(wParam) == 998) {
			EndDialog(hwnd, 0);

			//case  40022: {//De Pantallap principal a USUARIOS ACTUALIZAR
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG6), hwnd, listausuarios);
		}
		if (LOWORD(wParam) == 988)
		{
			//case 40018: {//Salir			
			int Opcion = MessageBox(0, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (Opcion)
			{
			case IDYES:
			{
				DestroyWindow(hwnd);
			}
			break;
			}
		}/*break;*/
		if (LOWORD(wParam) == 41) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {

				int indice = 0;

				indice = SendDlgItemMessage(hwnd, 41, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, 41, LB_GETTEXT, indice, (LPARAM)textoList);

				Usuario_aux = Usuario_inicio;
				while (Usuario_aux != nullptr && strcmp(textoList, Usuario_aux->nombreus) != 0) {
					Usuario_aux = Usuario_aux->Usuario_sig;
				}
				if (Usuario_aux == nullptr) {

				}
				else
				{
					SetDlgItemText(hwnd, 43, Usuario_aux->nombreus);
					SetDlgItemText(hwnd, 44, Usuario_aux->nombrecompleto);
					SetDlgItemText(hwnd, 42, Usuario_aux->comercious);
					SetDlgItemText(hwnd, 45, Usuario_aux->direccionus);
					SendDlgItemMessage(hwnd, 40, CB_SELECTSTRING, 0, (LPARAM)Usuario_aux->usuariotipo);
					HBITMAP bmp;
					bmp = (HBITMAP)LoadImage(NULL, Usuario_aux->fotous, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
					SendDlgItemMessage(hwnd, 46, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
				}

			}break;
			}
		}
		if (LOWORD(wParam) == 48) {//Registrar
			int indice = 0;
			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthNombre = 0;

			char Nnombre[51] = { 0 };
			char Ndireccion[100] = { 0 };
			char Nfoto[300] = { 0 };
			char Ntipo[20] = { 0 };
			//Usuario* newInfoUser = new Usuario;
			Usuario_aux = Usuario_inicio;
			indice = SendDlgItemMessage(hwnd, 41, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, 41, LB_GETTEXT, indice, (LPARAM)textoList);


			GetDlgItemText(hwnd, 44, Nnombre, sizeof(Nnombre));
			GetDlgItemText(hwnd, 40, Ntipo, sizeof(Ntipo));
			GetDlgItemText(hwnd, 46, Nfoto, sizeof(Nfoto));
			GetDlgItemText(hwnd, 45, Ndireccion, sizeof(Ndireccion));

			//Modificar_Usuario(newInfoUser, textoList);

			while (Usuario_aux != nullptr && strcmp(Usuario_aux->nombreus, textoList) != 0) {

				Usuario_aux = Usuario_aux->Usuario_sig;

			}
			if (Usuario_aux == nullptr) {
				MessageBeep(MB_OK);
				MessageBox(hwnd, "No se encontro usuario", "ADVERTENCIA", MB_OK | MB_ICONERROR);

			}
			else
			{
				lengthNombre = strlen(Nnombre);
				if (lengthNombre < 5 || lengthNombre > 50)
				{
					MessageBox(hwnd, "Favor de ingresar de 5 a 50 caracteres alfabeticos.", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
					/*	strcpy_s(Usuario_aux->Usuario_pv, "0");*/
				}
				else {
					for (int i = 0; i < lengthNombre; i++)
					{
						if (ispunct(Nnombre[i]))
						{
							CCaracteres++;
						}
						if (isdigit(Nnombre[i]))
						{
							CNumeros++;
						}
					}
					if (CCaracteres != 0 || CNumeros != 0)
					{
						MessageBox(hwnd, "Favor de solo ingresar caracteres alfabeticos.", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
						/*strcpy_s(NUsuario->Usuario_pv, "0");*/
					}
					else
					{
						int Opcion = MessageBox(0, "¿Seguro desea modificar el usuario?", "AVISO", MB_YESNO | MB_ICONQUESTION);
						switch (Opcion) {
						case IDYES: {
							if (strcmp(Nnombre, "") != 0 && strcmp(Ndireccion, "") != 0)
							{
								strcpy_s(Usuario_aux->primera, "1");
							}
							strcpy_s(Usuario_aux->nombrecompleto, Nnombre);
							strcpy_s(Usuario_aux->usuariotipo, Ntipo);
							strcpy_s(Usuario_aux->fotous, Ndireccion);
							strcpy_s(Usuario_aux->direccionus, Ndireccion);

							MessageBox(0, "Usuario modificado", "AVISO", MB_OK | MB_ICONINFORMATION);
							EndDialog(hwnd, 0);
							Escribir_Binario_Usuario();
							
							DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG6), hwnd, listausuarios);
						}
								  break;
						}

					}

				}
			}

			//SendMessage(GetDlgItem(hwnd, 1043), LB_RESETCONTENT, 0, 0);
			//SendMessage(GetDlgItem(hPrinGlob, 1043), LB_RESETCONTENT, 0, 0);

			//Usuario_aux = Usuario_inicio;
			//while (Usuario_aux != nullptr) {
			//	SendMessage(GetDlgItem(hwnd, 1043), LB_ADDSTRING, 0, (LPARAM)Usuario_aux->Usuario_nombre);
			//	SendMessage(GetDlgItem(hPrinGlob, 1043), LB_ADDSTRING, 0, (LPARAM)Usuario_aux->Usuario_nombre);
			//	Usuario_aux = Usuario_aux->Usuario_sig;
			//}

		}
		if (LOWORD(wParam) == 49) {//Eliminar
			int indice = 0;

			indice = SendDlgItemMessage(hwnd, 41, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, 41, LB_GETTEXT, indice, (LPARAM)textoList);

			if (strcmp(textoList, NUsuario->nombreus) == 0)
			{
				MessageBox(hwnd, "No se puede eliminar un usuario activo, favor de cerrar sesión y usar otro usuario", "AVISO", MB_OK | MB_ICONERROR);
			}
			else {
				int Opcion = MessageBox(0, "¿Seguro que desea eliminar al usuario?", "AVISO", MB_YESNO | MB_ICONQUESTION);
				if (Opcion == IDYES) {
					Eliminar_Usuario(textoList);

					SendMessage(GetDlgItem(hwnd, 41), LB_DELETESTRING, (WPARAM)indice, 0);
					SendMessage(GetDlgItem(hPrinGlob, 41), LB_RESETCONTENT, 0, 0);
					Escribir_Binario_Usuario();

					Usuario_aux = Usuario_inicio;
					while (Usuario_aux != nullptr) {
						SendMessage(GetDlgItem(hPrinGlob, 41), LB_ADDSTRING, 0, (LPARAM)Usuario_aux->nombreus);
						Usuario_aux = Usuario_aux->Usuario_sig;
					}
				}

			}

		}
		if (LOWORD(wParam) == 50) {//modificar
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP bmp; //1

				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE); //2
				SetDlgItemText(hwnd, 45, zFile);
				SendDlgItemMessage(hwnd, 46, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); //3 
			}
			else {
				MessageBox(hwnd, "No seleccionó foto", "AVISO", MB_OK | MB_ICONERROR);
				strcpy_s(zFile, NUsuario->fotous);

			}
		}
		if (LOWORD(wParam) == 52) {//modificar
			if (strcmp(NUsuario->usuariotipo, "Administrador") == 0)
			{
				EnableWindow(GetDlgItem(hwnd, 44), true);
				EnableWindow(GetDlgItem(hwnd, 40), true);
				EnableWindow(GetDlgItem(hwnd, 50), true);

			}
		}
		if (LOWORD(wParam) == 53) {//modificar
			EndDialog(hwnd, 0); DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, PantallaPrincipal);
		}
}	break;
	}
	
	
return false;
}
LRESULT CALLBACK usuariocontra(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	char textoList[50] = { 0 };
	switch (msg)
	{
	
	case WM_INITDIALOG:
	{//rellena lista de usuarios
	
		if (strcmp(NUsuario->usuariotipo, "Cajero") == 0)
		{
			EnableMenuItem(GetMenu(hwnd), 996, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), 999, MF_DISABLED | MF_BYCOMMAND);
			EnableMenuItem(GetMenu(hwnd), 993, MF_DISABLED | MF_BYCOMMAND);

		}
		char Usuario_comercio_c[15];
		strcpy_s(Usuario_comercio_c, NUsuario->comercious);

		Usuario_aux = Usuario_inicio;
		while (Usuario_aux != nullptr)
		{
			if (strcmp(Usuario_comercio_c, Usuario_aux->comercious) == 0)
			{
				SendMessage(GetDlgItem(hwnd, 61), LB_ADDSTRING, 0, (LPARAM)Usuario_aux->nombreus);

			}
			Usuario_aux = Usuario_aux->Usuario_sig;
		}

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->fotous, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 65, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 68, NUsuario->nombreus);
	}
	break;
	case WM_COMMAND: {
		if (LOWORD(wParam) == 61) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {

				int indice = 0;

				indice = SendDlgItemMessage(hwnd, 61, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, 61, LB_GETTEXT, indice, (LPARAM)textoList);

				Usuario_aux = Usuario_inicio;
				while (Usuario_aux != nullptr && strcmp(textoList, Usuario_aux->nombreus) != 0) {
					Usuario_aux = Usuario_aux->Usuario_sig;
				}
				if (Usuario_aux == nullptr) {

				}
				else
				{
					SetDlgItemText(hwnd, 63, Usuario_aux->contraus);
				}

			}break;
			}
		}
		if (LOWORD(wParam) == 66) {//Modificar
			int indice = 0;

			char NContra[50] = { 0 };
			//Usuario* newInfoUser = new Usuario;
			Usuario_aux = Usuario_inicio;
			indice = SendDlgItemMessage(hwnd, 61, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, 61, LB_GETTEXT, indice, (LPARAM)textoList);


			GetDlgItemText(hwnd, 64, NContra, sizeof(NContra));
			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthContrasena = 0;

			lengthContrasena = strlen(NContra);
			//Modificar_Usuario(newInfoUser, textoList);
			while (Usuario_aux != nullptr && strcmp(Usuario_aux->nombreus, textoList) != 0) {

				Usuario_aux = Usuario_aux->Usuario_sig;

			}
			if (Usuario_aux == nullptr) {
				MessageBeep(MB_OK);
				MessageBox(hwnd, "No hay usuario seleccionado", "ADVERTENCIA", MB_OK | MB_ICONERROR);
			}
			else
			{
				if (lengthContrasena < 3 || lengthContrasena>10)
				{
					MessageBox(hwnd, "Favor de ingresar de 3 a 10 caracteres. Minimo 1 letra, 1 numero y 1 caracter especial (Sin espacios).", "CONTRASEÑA", MB_OK | MB_ICONEXCLAMATION);
				}
				else {
					for (int i = 0; i < lengthContrasena; i++)
					{
						if (isalpha(NContra[i]))
						{
							CLetras++;
						}
						if (ispunct(NContra[i]))
						{
							CCaracteres++;
						}
						if (isdigit(NContra[i]))
						{
							CNumeros++;
						}
						if (isspace(NContra[i]))
						{
							CEspacios++;
						}
					}
					if (CCaracteres == 0 || CNumeros == 0 || CLetras == 0 || CEspacios != 0)
					{
						MessageBox(hwnd, "Favor de ingresar minimo 1 letra, 1 numero y 1 caracter especial(Sin espacios).", "CONTRASEÑA", MB_OK | MB_ICONEXCLAMATION);
					}
					else
					{
						strcpy_s(Usuario_aux->contraus, NContra);
						MessageBox(0, "Contraseña modificadaa", "AVISO", MB_OK | MB_ICONINFORMATION);
						Escribir_Binario_Usuario();
						EndDialog(hwnd, 0);
						DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG7), hwnd, usuariocontra);
					}
				}


			}
		}

		if (LOWORD(wParam) == 67) {
			//case 40018: {//Salir			
			int Opcion = MessageBox(0, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			switch (Opcion) {
			case IDYES: {
				EndDialog(hwnd, 0);
				DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, PantallaPrincipal);
			}
					  break;
			}

		}/*break;*/
	}break;

	}return false;
}


LRESULT CALLBACK clientealta(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	
	case WM_INITDIALOG: {
		
		SYSTEMTIME fechaHoy;
		ZeroMemory(&fechaHoy, sizeof(fechaHoy));
		GetLocalTime(&fechaHoy);

		char fechaCompleta[11] = { 0 };
		char charFecha[5] = { 0 };

		sprintf_s(charFecha, "%d", fechaHoy.wDay);
		strcat_s(fechaCompleta, charFecha);
		strcat_s(fechaCompleta, "/");
		sprintf_s(charFecha, "%d", fechaHoy.wMonth);
		strcat_s(fechaCompleta, charFecha);
		strcat_s(fechaCompleta, "/");
		sprintf_s(charFecha, "%d", fechaHoy.wYear);
		strcat_s(fechaCompleta, charFecha);


		SetDlgItemText(hwnd, 74, fechaCompleta);
		SetDlgItemText(hwnd, 73, NUsuario->comercious);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->fotous, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 75, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 76, NUsuario->nombreus);
	}break;
	case WM_COMMAND: 
	{
		switch (LOWORD(wParam)) 
		{
		case 78: {//Regresar a menu
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, PantallaPrincipal);
		}break;

		case 77: { //Boton de Guardar
			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthNombre = 0;
			int lengthEmail = 0;

			Cliente* Nuevo_cliente = new Cliente;
			GetDlgItemText(hwnd, 71, Nuevo_cliente->nombrecliente, sizeof(Nuevo_cliente->nombrecliente));
			GetDlgItemText(hwnd, 72, Nuevo_cliente->clienteemail, sizeof(Nuevo_cliente->clienteemail));
			GetDlgItemText(hwnd, 73, Nuevo_cliente->comerciocliente, sizeof(Nuevo_cliente->comerciocliente));
			GetDlgItemText(hwnd, 74, Nuevo_cliente->fechacliente, sizeof(Nuevo_cliente->fechacliente));
			Cliente_aux = Cliente_inicio;
			lengthNombre = strlen(Nuevo_cliente->nombrecliente);
			lengthEmail = strlen(Nuevo_cliente->clienteemail);

			if (lengthNombre < 5 || lengthNombre > 50)
			{
				MessageBox(hwnd, "Favor de ingresar de un maximo de 15 caracteres alfabeticos.", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
			}
			else {
				for (int i = 0; i < lengthNombre; i++)
				{
					if (ispunct(Nuevo_cliente->nombrecliente[i]))
					{
						CCaracteres++;
					}
					if (isdigit(Nuevo_cliente->nombrecliente[i]))
					{
						CNumeros++;
					}
				}
				if (CCaracteres != 0 || CNumeros != 0)
				{
					MessageBox(hwnd, "Favor de solo ingresar caracteres alfabeticos.", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
				}
				else
				{
					while (Cliente_aux != nullptr && strcmp(Cliente_aux->nombrecliente, Nuevo_cliente->nombrecliente) != 0) {//validacion mismmo nombre
						Cliente_aux = Cliente_aux->Cliente_sig;
					}
					if (Cliente_aux != nullptr) {
						MessageBox(hwnd, "Cliente ya existente, intente otro nombre", "AVISO", MB_OK | MB_ICONERROR);
					}
					else {
						if (lengthEmail < 5 || lengthEmail > 25)//validacion email
						{
							MessageBox(hwnd, "Favor de ingresar de 5 a 25 caracteres.", "EMAIL", MB_OK | MB_ICONEXCLAMATION);
						}
						else {
							for (int i = 0; i < lengthEmail; i++)
							{
								if (Nuevo_cliente->clienteemail[i] == '@')
								{
									CCaracteres++;
								}
							}
							if (CCaracteres == 0)
							{
								MessageBox(hwnd, "Email invalido.", "EMAIL", MB_OK | MB_ICONEXCLAMATION);
							}
							else
							{
								Agregar_Cliente(Nuevo_cliente);	

								delete Nuevo_cliente;
							Escribir_Binario_Cliente();

								SetDlgItemText(hwnd, 71, "");
								SetDlgItemText(hwnd, 72, "");
							}
						}
					}

				}
			}
		}break;
		}
	}break;
	}
	return false;
}
LRESULT CALLBACK actualizarcliente(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
	{
	char textoList[50] = { 0 };
	switch (msg) {
	case WM_INITDIALOG: {//rellena lista de usuarios
		if (strcmp(NUsuario->usuariotipo, "Cajero") == 0)
		{
			EnableWindow(GetDlgItem(hwnd, 88), false);
		}

		char Usuario_comercio_c[15];
		strcpy_s(Usuario_comercio_c, NUsuario->comercious);

		Cliente_aux = Cliente_inicio;
		while (Cliente_aux != nullptr) {
			if (strcmp(Usuario_comercio_c, Cliente_aux->comerciocliente) == 0) {
				SendMessage(GetDlgItem(hwnd, 80), LB_ADDSTRING, 0, (LPARAM)Cliente_aux->nombrecliente);
			}
			Cliente_aux = Cliente_aux->Cliente_sig;
		}
		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->fotous, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 85, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 86, NUsuario->nombreus);
	}break;
	case WM_COMMAND: {
		if (LOWORD(wParam) == 80) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {

				int indice = 0;

				indice = SendDlgItemMessage(hwnd, 80, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, 80, LB_GETTEXT, indice, (LPARAM)textoList);

				Cliente_aux = Cliente_inicio;
				while (Cliente_aux != nullptr && strcmp(textoList, Cliente_aux->nombrecliente) != 0) {
					Cliente_aux = Cliente_aux->Cliente_sig;
				}
				if (Cliente_aux == nullptr) {

				}
				else
				{
					SetDlgItemText(hwnd, 81, Cliente_aux->nombrecliente);
					SetDlgItemText(hwnd, 82, Cliente_aux->clienteemail);
					SetDlgItemText(hwnd, 83, Cliente_aux->comerciocliente);
					SetDlgItemText(hwnd, 84, Cliente_aux->fechacliente);
				}

			}break;
			}
		}
		if (LOWORD(wParam) == 89) {//Registrar
			int indice = 0;
			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthEmail = 0;

			char NEmail[51] = { 0 };
			//Usuario* newInfoUser = new Usuario;
			Cliente_aux = Cliente_inicio;
			indice = SendDlgItemMessage(hwnd, 80, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd,80, LB_GETTEXT, indice, (LPARAM)textoList);


			GetDlgItemText(hwnd, 82, NEmail, sizeof(NEmail));

			//Modificar_Usuario(newInfoUser, textoList);


			while (Cliente_aux != nullptr && strcmp(Cliente_aux->nombrecliente, textoList) != 0) {

				Cliente_aux = Cliente_aux->Cliente_sig;

			}
			if (Cliente_aux == nullptr) {
				MessageBeep(MB_OK);
				MessageBox(hwnd, "No se encontro el cliente", "ADVERTENCIA", MB_OK | MB_ICONERROR);

			}
			else {
				lengthEmail = strlen(NEmail);
				if (lengthEmail < 5 || lengthEmail > 25)//validacion email
				{
					MessageBox(hwnd, "Favor de ingresar de 5 a 25 caracteres.", "EMAIL", MB_OK | MB_ICONEXCLAMATION);
				}
				else {
					for (int i = 0; i < lengthEmail; i++)
					{
						if (NEmail[i] == '@')
						{
							CCaracteres++;
						}
					}
					if (CCaracteres == 0)
					{
						MessageBox(hwnd, "Email invalido.", "EMAIL", MB_OK | MB_ICONEXCLAMATION);
					}
					else
					{
						int Opcion = MessageBox(0, "¿Seguro desea modificar el cliente?", "AVISO", MB_YESNO | MB_ICONQUESTION);
						switch (Opcion) {
						case IDYES: {
							strcpy_s(Cliente_aux->clienteemail, NEmail);
							MessageBox(0, "Cliente modificado", "AVISO", MB_OK | MB_ICONINFORMATION);
							EndDialog(hwnd, 0);
							Escribir_Binario_Cliente();
							DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG9), hwnd, actualizarcliente);
						}
								  break;
						}

					}

				}
			}



		}


		if (LOWORD(wParam) == 88) {//Eliminar
			int indice = 0;

			indice = SendDlgItemMessage(hwnd, 80, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, 80, LB_GETTEXT, indice, (LPARAM)textoList);

			int Opcion = MessageBox(0, "¿Seguro que desea eliminar el cliente?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			if (Opcion == IDYES) {
				Eliminar_Cliente(textoList);

				SendMessage(GetDlgItem(hwnd, 80), LB_DELETESTRING, (WPARAM)indice, 0);
				SendMessage(GetDlgItem(hPrinGlob, 80), LB_RESETCONTENT, 0, 0);
				Escribir_Binario_Cliente();

				Cliente_aux = Cliente_inicio;
				while (Cliente_aux != nullptr) {
					SendMessage(GetDlgItem(hPrinGlob, 80), LB_ADDSTRING, 0, (LPARAM)Cliente_aux->comerciocliente);
					Cliente_aux = Cliente_aux->Cliente_sig;
				}
			}

		}
		if (LOWORD(wParam) == 87) {
			
				EnableWindow(GetDlgItem(hwnd, 82), true);
			
		}

		if (LOWORD(wParam) == 90) {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, PantallaPrincipal);
		}
		//switch (LOWORD(wParam)) {
		//case 1065: {//Regresar a menu
		//	EndDialog(hwnd, 0);
		//}break;
		//}
	}break;
	}
	return false;
}


LRESULT CALLBACK altaconsumo(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	char textoList[50] = { 0 };
	switch (msg) 
	{

	case WM_INITDIALOG: 

	
	{//rellena lista de CLIENTES

		char Usuario_comercio_c[15];
		strcpy_s(Usuario_comercio_c, NUsuario->comercious);

		Cliente_aux = Cliente_inicio;
		while (Cliente_aux != nullptr) {
			if (strcmp(Usuario_comercio_c, Cliente_aux->comerciocliente) == 0) {
				SendMessage(GetDlgItem(hwnd, 91), LB_ADDSTRING, 0, (LPARAM)Cliente_aux->nombrecliente);
			}
			Cliente_aux = Cliente_aux->Cliente_sig;
		}


		SYSTEMTIME fechaHoy;
		ZeroMemory(&fechaHoy, sizeof(fechaHoy));
		GetLocalTime(&fechaHoy);

		char fechaCompleta[50] = { 0 };
		char charFecha[50] = { 0 };
		float CantidadMonto = 0;

		char FC[50] = { 0 };
		char Fechaf[50] = { 0 };

		sprintf_s(charFecha, "%d", fechaHoy.wDay);
		strcat_s(fechaCompleta, charFecha);
		strcat_s(fechaCompleta, "/");
		sprintf_s(charFecha, "%d", fechaHoy.wMonth);
		strcat_s(fechaCompleta, charFecha);
		strcat_s(fechaCompleta, "/");
		sprintf_s(charFecha, "%d", fechaHoy.wYear);
		strcat_s(fechaCompleta, charFecha);

		sprintf_s(Fechaf, "%d", fechaHoy.wDay);
		strcat_s(FC, Fechaf);
		strcat_s(FC, "/");
		sprintf_s(Fechaf, "%d", fechaHoy.wMonth);
		strcat_s(FC, Fechaf);
		strcat_s(FC, "/");
		sprintf_s(Fechaf, "%d", fechaHoy.wYear);
		strcat_s(FC, Fechaf);
		strcat_s(FC, " ");
		sprintf_s(Fechaf, "%d", fechaHoy.wHour);
		strcat_s(FC, Fechaf);
		strcat_s(FC, ":");
		sprintf_s(Fechaf, "%d", fechaHoy.wMinute);
		strcat_s(FC, Fechaf);
		strcat_s(FC, ":");
		sprintf_s(Fechaf, "%d", fechaHoy.wSecond);
		strcat_s(FC, Fechaf);

		SetDlgItemText(hwnd, 95, fechaCompleta);
		SetDlgItemText(hwnd, 100, NUsuario->comercious);
		SetDlgItemText(hwnd, 102, FC);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->fotous, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 96, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 97, NUsuario->nombreus);
	}break;

	{
		if (LOWORD(wParam) == 91) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {

				int indice = 0;

				indice = SendDlgItemMessage(hwnd, 91, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, 91, LB_GETTEXT, indice, (LPARAM)textoList);

				Cliente_aux = Cliente_inicio;
				while (Cliente_aux != nullptr && strcmp(textoList, Cliente_aux->nombrecliente) != 0) {
					Cliente_aux = Cliente_aux->Cliente_sig;
				}
				if (Cliente_aux == nullptr) {

				}
				else
				{
					SetDlgItemText(hwnd, 104, Cliente_aux->nombrecliente);
				}

			}break;
			}
		}

		if (LOWORD(wParam) == 98) {
			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthMonto = 0;
			float CantidadMonto = 0;

			char Usuario_comercio_c[15];


			float CantidadDM = 0;
			float CantidadDescuento = 0;
			float CantidadSubtotal = 0;
			float CantidadTotal = 0;
			float Fmonto = 0;
			char Descuento[11] = { 0 };
			char Subtotal[11] = { 0 };
			char Total[11] = { 0 };


			int Validacion = 0;



			Consumo* Nuevo_consumo = new Consumo;
			GetDlgItemText(hwnd,104, Nuevo_consumo->Concliente, sizeof(Nuevo_consumo->Concliente));
			GetDlgItemText(hwnd,92, Nuevo_consumo->Conmonto, sizeof(Nuevo_consumo->Conmonto));
			GetDlgItemText(hwnd, 100, Nuevo_consumo->Concomercio, sizeof(Nuevo_consumo->Concomercio));
			GetDlgItemText(hwnd, 95, Nuevo_consumo->Confecha, sizeof(Nuevo_consumo->Confecha));
			GetDlgItemText(hwnd, 102, Nuevo_consumo->ConID, sizeof(Nuevo_consumo->ConID));

			CantidadMonto = atof(Nuevo_consumo->Conmonto);
			lengthMonto = strlen(Nuevo_consumo->Conmonto);

			strcpy_s(Usuario_comercio_c, NUsuario->comercious);
			char CCM[50] = { 0 };
			GetDlgItemText(hwnd, 92, CCM, sizeof(CCM));
			Fmonto = atof(CCM);
			lengthMonto = strlen(CCM);

			if (strcmp(Nuevo_consumo->Concliente, "") == 0)
			{
				MessageBox(hwnd, "Favor de seleccionar un cliente de la lista dando doble click.", "CLIENTE", MB_OK | MB_ICONEXCLAMATION);
			}
			else {
				if (CantidadMonto <= 0)
				{
					MessageBox(hwnd, "Favor de ingresar numeros mayores a 0.", "MONTO", MB_OK | MB_ICONEXCLAMATION);
					SetDlgItemText(hwnd, 94, "");
					SetDlgItemText(hwnd, 107, "");
					SetDlgItemText(hwnd, 93, "");
				}
				else {
					for (int i = 0; i < lengthMonto; i++)
					{
						if (ispunct(Nuevo_consumo->Conmonto[i]))
						{
							CCaracteres++;
						}
						if (isalpha(Nuevo_consumo->Conmonto[i]))
						{
							CLetras++;
						}
						if (isspace(Nuevo_consumo->Conmonto[i]))
						{
							CEspacios++;
						}
						if (Nuevo_consumo->Conmonto[i] == '.')
						{
							CCaracteres = CCaracteres - 1;
						}
					}
					if (CCaracteres != 0 || CLetras != 0 || CEspacios != 0)
					{
						MessageBox(hwnd, "Favor de solo ingresar numeros.", "MONTO", MB_OK | MB_ICONEXCLAMATION);
						SetDlgItemText(hwnd, 94, "");
						SetDlgItemText(hwnd, 107, "");
						SetDlgItemText(hwnd, 93, "");
					}
					else
					{
						Promocion_aux = Promocion_inicio;
						while (Promocion_aux != nullptr) {
							if (strcmp(Usuario_comercio_c, Promocion_aux->Promcomercio) == 0) //si el comercio del usuario coincide con el comercio promocon
							{
								if (strcmp(CCM, Promocion_aux->Prommonto) == 0) {//si el monto consumo coincide con el promocion

									if (strcmp(Promocion_aux->Promestatus, "Activa") == 0) {

										CantidadMonto = atof(CCM);
										CantidadDescuento = atof(Promocion_aux->Promdescuento);
										CantidadDM = CantidadMonto * (CantidadDescuento / 100);
										CantidadDescuento = (100 - CantidadDescuento) / 100;
										CantidadSubtotal = CantidadMonto * CantidadDescuento;
										CantidadTotal = CantidadSubtotal + (CantidadSubtotal * 0.16);

										sprintf_s(Descuento, "%.2f", CantidadDM);
										SetDlgItemText(hwnd, 107, Descuento);

										sprintf_s(Subtotal, "%.2f", CantidadSubtotal);
										SetDlgItemText(hwnd, 93, Subtotal);

										sprintf_s(Total, "%.2f", CantidadTotal);
										SetDlgItemText(hwnd, 94, Total);
										Validacion = 1;
									}
								}
							}
							Promocion_aux = Promocion_aux->Promocion_sig;
						}
						if (Validacion == 0)
						{
							CantidadMonto = atof(CCM);
							CantidadDM = 0;
							CantidadSubtotal = CantidadMonto;
							CantidadTotal = CantidadSubtotal + (CantidadSubtotal * 0.16);
							sprintf_s(Descuento, "%.2f", CantidadDM);
							SetDlgItemText(hwnd, 107, Descuento);

							sprintf_s(Subtotal, "%.2f", CantidadSubtotal);
							SetDlgItemText(hwnd, 93, Subtotal);

							sprintf_s(Total, "%.2f", CantidadTotal);
							SetDlgItemText(hwnd, 94, Total);
						}

						GetDlgItemText(hwnd, 93, Nuevo_consumo->Consubtotal, sizeof(Nuevo_consumo->Consubtotal));
						GetDlgItemText(hwnd, 107, Nuevo_consumo->Condescuento, sizeof(Nuevo_consumo->Condescuento));
						GetDlgItemText(hwnd, 94, Nuevo_consumo->Contotal, sizeof(Nuevo_consumo->Contotal));

						Agregar_Consumo(Nuevo_consumo);
						delete Nuevo_consumo;
						Escribir_Binario_Consumo();

						SYSTEMTIME fechaHoy;
						ZeroMemory(&fechaHoy, sizeof(fechaHoy));
						GetLocalTime(&fechaHoy);
						SetDlgItemText(hwnd, 94, "");
						SetDlgItemText(hwnd, 107, "");
						SetDlgItemText(hwnd, 93, "");
						SetDlgItemText(hwnd, 104, "");
						SetDlgItemText(hwnd, 92, "");
						char FC[50] = { 0 };
						char Fechaf[50] = { 0 };
						sprintf_s(Fechaf, "%d", fechaHoy.wDay);
						strcat_s(FC, Fechaf);
						strcat_s(FC, "/");
						sprintf_s(Fechaf, "%d", fechaHoy.wMonth);
						strcat_s(FC, Fechaf);
						strcat_s(FC, "/");
						sprintf_s(Fechaf, "%d", fechaHoy.wYear);
						strcat_s(FC, Fechaf);
						strcat_s(FC, " ");
						sprintf_s(Fechaf, "%d", fechaHoy.wHour);
						strcat_s(FC, Fechaf);
						strcat_s(FC, ":");
						sprintf_s(Fechaf, "%d", fechaHoy.wMinute);
						strcat_s(FC, Fechaf);
						strcat_s(FC, ":");
						sprintf_s(Fechaf, "%d", fechaHoy.wSecond);
						strcat_s(FC, Fechaf);
						SetDlgItemText(hwnd, 102, FC);
					}
				}
			}

		}
		if (LOWORD(wParam) == 108) { //calcular
			char Usuario_comercio_c[15];
			float CantidadMonto = 0;
			float CantidadDM = 0;
			float CantidadDescuento = 0;
			float CantidadSubtotal = 0;
			float CantidadTotal = 0;
			float Fmonto = 0;
			char Descuento[11] = { 0 };
			char Subtotal[11] = { 0 };
			char Total[11] = { 0 };

			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthMonto = 0;
			int Validacion = 0;


			strcpy_s(Usuario_comercio_c, NUsuario->comercious);
			char CCM[50] = { 0 };
			GetDlgItemText(hwnd, 92, CCM, sizeof(CCM));
			Fmonto = atof(CCM);
			lengthMonto = strlen(CCM);



			if (Fmonto <= 0)
			{
				MessageBox(hwnd, "Favor de ingresar numeros mayores a 0.", "MONTO", MB_OK | MB_ICONEXCLAMATION);
				SetDlgItemText(hwnd, 94, "");
				SetDlgItemText(hwnd, 107, "");
				SetDlgItemText(hwnd, 93, "");
			}
			else {
				for (int i = 0; i < lengthMonto; i++)
				{
					if (ispunct(CCM[i]))
					{
						CCaracteres++;
					}
					if (isalpha(CCM[i]))
					{
						CLetras++;
					}
					if (isspace(CCM[i]))
					{
						CEspacios++;
					}
					if (CCM[i] == '.')
					{
						CCaracteres = CCaracteres - 1;
					}
				}
				if (CCaracteres != 0 || CLetras != 0 || CEspacios != 0)
				{
					MessageBox(hwnd, "Favor de solo ingresar numeros.", "MONTO", MB_OK | MB_ICONEXCLAMATION);
				}
				else
				{
					Promocion_aux = Promocion_inicio;
					while (Promocion_aux != nullptr) {
						if (strcmp(Usuario_comercio_c, Promocion_aux->Promcomercio) == 0) //si el comercio del usuario coincide con el comercio promocon
						{
							if (strcmp(CCM, Promocion_aux->Prommonto) == 0) {//si el monto consumo coincide con el promocion

								if (strcmp(Promocion_aux->Promestatus, "Activa") == 0) {

									CantidadMonto = atof(CCM);
									CantidadDescuento = atof(Promocion_aux->Promdescuento);
									CantidadDM = CantidadMonto * (CantidadDescuento / 100);
									CantidadDescuento = (100 - CantidadDescuento) / 100;
									CantidadSubtotal = CantidadMonto * CantidadDescuento;
									CantidadTotal = CantidadSubtotal + (CantidadSubtotal * 0.16);

									sprintf_s(Descuento, "%.2f", CantidadDM);
									SetDlgItemText(hwnd, 107, Descuento);

									sprintf_s(Subtotal, "%.2f", CantidadSubtotal);
									SetDlgItemText(hwnd, 93, Subtotal);

									sprintf_s(Total, "%.2f", CantidadTotal);
									SetDlgItemText(hwnd, 94, Total);
									Validacion = 1;
								}
							}
						}
						Promocion_aux = Promocion_aux->Promocion_sig;
					}
					if (Validacion == 0)
					{
						CantidadMonto = atof(CCM);
						CantidadDM = 0;
						CantidadSubtotal = CantidadMonto;
						CantidadTotal = CantidadSubtotal + (CantidadSubtotal * 0.16);
						sprintf_s(Descuento, "%.2f", CantidadDM);
						SetDlgItemText(hwnd, 107, Descuento);

						sprintf_s(Subtotal, "%.2f", CantidadSubtotal);
						SetDlgItemText(hwnd, 93, Subtotal);

						sprintf_s(Total, "%.2f", CantidadTotal);
						SetDlgItemText(hwnd, 94, Total);
					}
				}


			}

			/*}*/


		}

		if (LOWORD(wParam) == 99) {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, PantallaPrincipal);
		}

	}break;
	}
	return false;
}
LRESULT CALLBACK listaconsumo(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	char textoList[50] = { 0 };
	switch (msg) {

	case WM_INITDIALOG: 
	{//rellena lista de usuarios
		
		char Usuario_comercio_c[15];
		strcpy_s(Usuario_comercio_c, NUsuario->comercious);

		Consumo_aux = Consumo_inicio;
		while (Consumo_aux != nullptr) {
			if (strcmp(Usuario_comercio_c, Consumo_aux->Concomercio) == 0) {
				SendMessage(GetDlgItem(hwnd, 101), LB_ADDSTRING, 0, (LPARAM)Consumo_aux->ConID);
			}
			Consumo_aux = Consumo_aux->Consumo_sig;
		}

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->fotous, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 108, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 109, NUsuario->nombreus);
	}break;

	case WM_COMMAND: {
		if (LOWORD(wParam) == 101) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {

				int indice = 0;

				indice = SendDlgItemMessage(hwnd, 101, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, 101, LB_GETTEXT, indice, (LPARAM)textoList);

				Consumo_aux = Consumo_inicio;
				while (Consumo_aux != nullptr && strcmp(textoList, Consumo_aux->ConID) != 0) {
					Consumo_aux = Consumo_aux->Consumo_sig;
				}
				if (Consumo_aux == nullptr) {

				}
				else
				{
					SetDlgItemText(hwnd, 102, Consumo_aux->Concliente);
					SetDlgItemText(hwnd, 103, Consumo_aux->Conmonto);
					SetDlgItemText(hwnd, 105, Consumo_aux->Condescuento);
					SetDlgItemText(hwnd, 106, Consumo_aux->Contotal);
					SetDlgItemText(hwnd, 104, Consumo_aux->Consubtotal);
					SetDlgItemText(hwnd, 107, Consumo_aux->Confecha);
				}

			}break;
			}
		}

		if (LOWORD(wParam) == 110) {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, PantallaPrincipal);
		}
	}break;
	}
	return false;
}
LRESULT CALLBACK filtro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)	
{
	char textoList[50] = { 0 };
	switch (msg) {
	case WM_INITDIALOG: {//rellena lista de usuarios
		char Usuario_comercio_c[15];
		strcpy_s(Usuario_comercio_c, NUsuario->comercious);

		Cliente_aux = Cliente_inicio;
		while (Cliente_aux != nullptr) {
			if (strcmp(Usuario_comercio_c, Cliente_aux->comerciocliente) == 0) {
				SendMessage(GetDlgItem(hwnd, 110), LB_ADDSTRING, 0, (LPARAM)Cliente_aux->nombrecliente);
			}
			Cliente_aux = Cliente_aux->Cliente_sig;
		}

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->fotous, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 118, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 119, NUsuario->nombreus);
		}break;

	case WM_COMMAND: {
		if (LOWORD(wParam) == 110) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {

				SendMessage(GetDlgItem(hwnd, 11), LB_RESETCONTENT, 0, 0);
				SetDlgItemText(hwnd, 112, "");
				SetDlgItemText(hwnd, 113, "");
				SetDlgItemText(hwnd, 114, "");
				SetDlgItemText(hwnd, 115, "");
				SetDlgItemText(hwnd, 117, "");
				SetDlgItemText(hwnd, 116, "");
				int indice = 0;

				indice = SendDlgItemMessage(hwnd, 110, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, 110, LB_GETTEXT, indice, (LPARAM)textoList);

				Consumo_aux = Consumo_inicio;
				while (Consumo_aux != nullptr) {
					if (strcmp(textoList, Consumo_aux->Concliente) == 0)
					{
						SendMessage(GetDlgItem(hwnd, 11), LB_ADDSTRING, 0, (LPARAM)Consumo_aux->ConID);
					}
					Consumo_aux = Consumo_aux->Consumo_sig;
				}

			}break;
			}
		}

		if (LOWORD(wParam) == 11) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {



				int indice = 0;

				indice = SendDlgItemMessage(hwnd, 11, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, 11, LB_GETTEXT, indice, (LPARAM)textoList);

				Consumo_aux = Consumo_inicio;
				while (Consumo_aux != nullptr && strcmp(textoList, Consumo_aux->ConID) != 0) {
					Consumo_aux = Consumo_aux->Consumo_sig;
				}
				if (Consumo_aux == nullptr) {

				}
				else
				{
					SetDlgItemText(hwnd, 112, Consumo_aux->Concliente);
					SetDlgItemText(hwnd, 113, Consumo_aux->Conmonto);
					SetDlgItemText(hwnd, 114, Consumo_aux->Consubtotal);
					SetDlgItemText(hwnd, 115, Consumo_aux->Condescuento);
					SetDlgItemText(hwnd, 117, Consumo_aux->Confecha);
					SetDlgItemText(hwnd, 116, Consumo_aux->Contotal);
				}

			}break;
			}
		}
			
		if (LOWORD(wParam) == 120) {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, PantallaPrincipal);
		}

	}break;
	}
	return false;
}


LRESULT CALLBACK altapromocion(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		SYSTEMTIME fechaHoy;
		ZeroMemory(&fechaHoy, sizeof(fechaHoy));
		GetLocalTime(&fechaHoy);

		char fechaCompleta[11] = { 0 };
		char charFecha[5] = { 0 };

		sprintf_s(charFecha, "%d", fechaHoy.wDay);
		strcat_s(fechaCompleta, charFecha);
		strcat_s(fechaCompleta, "/");
		sprintf_s(charFecha, "%d", fechaHoy.wMonth);
		strcat_s(fechaCompleta, charFecha);
		strcat_s(fechaCompleta, "/");
		sprintf_s(charFecha, "%d", fechaHoy.wYear);
		strcat_s(fechaCompleta, charFecha);


		SetDlgItemText(hwnd, 124, fechaCompleta);
		SetDlgItemText(hwnd, 131, NUsuario->comercious);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->fotous, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 127, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 128, NUsuario->nombreus);
	}break;

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case 130: {//Regresar a menu
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, PantallaPrincipal);
		}break;
		case 129: { //Boton de Guardar
			int CLetras = 0;
			int CNumeros = 0;
			int CCaracteres = 0;
			int CEspacios = 0;
			int lengthNombre = 0;
			int lengthMonto = 0;
			int lengthDescuento = 0;
			float CantidadMonto = 0;
			float Descuento = 0;
			Promocion* Nuevo_promocion = new Promocion;
			GetDlgItemText(hwnd, 121, Nuevo_promocion->Promnombre, sizeof(Nuevo_promocion->Promnombre));
			GetDlgItemText(hwnd, 123, Nuevo_promocion-> Promdescuento, sizeof(Nuevo_promocion-> Promdescuento));
			GetDlgItemText(hwnd, 122, Nuevo_promocion->Prommonto, sizeof(Nuevo_promocion->Prommonto));
			GetDlgItemText(hwnd, 124, Nuevo_promocion->Promfecha, sizeof(Nuevo_promocion->Promfecha));
			GetDlgItemText(hwnd, 131, Nuevo_promocion->Promcomercio, sizeof(Nuevo_promocion->Promcomercio));
			CantidadMonto = atof(Nuevo_promocion->Prommonto);
			Descuento = atof(Nuevo_promocion-> Promdescuento);
			lengthNombre = strlen(Nuevo_promocion->Promnombre);
			lengthMonto = strlen(Nuevo_promocion->Prommonto);
			lengthDescuento = strlen(Nuevo_promocion-> Promdescuento);





			if (lengthNombre < 5 || lengthNombre > 50)
			{
				MessageBox(hwnd, "Favor de ingresar de 5 a 15 caracteres alfabeticos y/o espacios.", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
			}
			else {
				for (int i = 0; i < lengthNombre; i++)
				{
					if (ispunct(Nuevo_promocion->Promnombre[i]))
					{
						CCaracteres++;
					}
					if (isdigit(Nuevo_promocion->Promnombre[i]))
					{
						CNumeros++;
					}
				}
				if (CCaracteres != 0 || CNumeros != 0)
				{
					MessageBox(hwnd, "Favor de solo ingresar caracteres alfabeticos y/o espacios.", "NOMBRE", MB_OK | MB_ICONEXCLAMATION);
				}
				else
				{
					if (CantidadMonto < 100)
					{
						MessageBox(hwnd, "Solo numeros no negativos mayores a 100", "MONTO", MB_OK | MB_ICONEXCLAMATION);
					}
					else {
						for (int i = 0; i < lengthMonto; i++)
						{
							if (ispunct(Nuevo_promocion->Prommonto[i]))
							{
								CCaracteres++;
							}
							if (isalpha(Nuevo_promocion->Prommonto[i]))
							{
								CLetras++;
							}
							if (isspace(Nuevo_promocion->Prommonto[i]))
							{
								CEspacios++;
							}
							if (Nuevo_promocion->Prommonto[i] == '.')
							{
								CCaracteres = CCaracteres - 1;
							}
						}
						if (CCaracteres != 0 || CLetras != 0 || CEspacios != 0)
						{
							MessageBox(hwnd, "Favor de solo ingresar numeros.", "MONTO", MB_OK | MB_ICONEXCLAMATION);
						}
						else {
							if (Descuento <= 0 || Descuento > 100)
							{
								MessageBox(hwnd, "Ingresar cantidad decimal, mayor a 0 y menor a 100", "DESCUENTO", MB_OK | MB_ICONEXCLAMATION);
							}
							else {
								for (int i = 0; i < lengthDescuento; i++)
								{
									if (ispunct(Nuevo_promocion-> Promdescuento[i]))
									{
										CCaracteres++;
									}
									if (isalpha(Nuevo_promocion-> Promdescuento[i]))
									{
										CLetras++;
									}
									if (isspace(Nuevo_promocion-> Promdescuento[i]))
									{
										CEspacios++;
									}
									if (Nuevo_promocion-> Promdescuento[i] == '.')
									{
										CCaracteres = CCaracteres - 1;
									}
								}
								if (CCaracteres != 0 || CLetras != 0 || CEspacios != 0)
								{
									MessageBox(hwnd, "Favor de solo ingresar numeros.", "DESCUENTO", MB_OK | MB_ICONEXCLAMATION);
								}
								else {
									if (IsDlgButtonChecked(hwnd, 125) == BST_UNCHECKED && IsDlgButtonChecked(hwnd, 126) == BST_UNCHECKED)
									{
										MessageBox(hwnd, "Favor de seleccionar un estatus.", "ESTATUS", MB_OK | MB_ICONEXCLAMATION);
									}
									else {
										if (IsDlgButtonChecked(hwnd, 125) == BST_CHECKED)
										{
											strcpy_s(Nuevo_promocion->Promestatus, "Activa");
										}
										if (IsDlgButtonChecked(hwnd, 126) == BST_CHECKED)
										{
											strcpy_s(Nuevo_promocion->Promestatus, "Inactiva");
										}
										Agregar_Promocion(Nuevo_promocion);
										delete Nuevo_promocion;
										Escribir_Binario_Promocion();

										SetDlgItemText(hwnd, 121, "");
										SetDlgItemText(hwnd, 123, "");
										SetDlgItemText(hwnd, 122, "");
										SendDlgItemMessage(hwnd, 126, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
										SendDlgItemMessage(hwnd, 125, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);

									}
								}

							}
						}
					}
				}
			}



		}break;
		}
	}break;
	}
	return false;
}
LRESULT CALLBACK listapromo(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	char textoList[50] = { 0 };
	switch (msg) {
	case WM_INITDIALOG: {
		char Usuario_comercio_c[15];
		strcpy_s(Usuario_comercio_c, NUsuario->comercious);

		if (strcmp(NUsuario->usuariotipo, "Cajero") == 0)
		{
			EnableWindow(GetDlgItem(hwnd, 150), false);
			EnableWindow(GetDlgItem(hwnd, 151), false);
			EnableWindow(GetDlgItem(hwnd, 152), false);
		}

		Promocion_aux = Promocion_inicio;
		while (Promocion_aux != nullptr) {
			if (strcmp(Usuario_comercio_c, Promocion_aux->Promcomercio) == 0) {
				SendMessage(GetDlgItem(hwnd, 141), LB_ADDSTRING, 0, (LPARAM)Promocion_aux->Promnombre);
			}
			Promocion_aux = Promocion_aux->Promocion_sig;
		}

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, NUsuario->fotous, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, 148, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);
		SetDlgItemText(hwnd, 149, NUsuario->nombreus);

	}break;
	case WM_COMMAND: {
		if (LOWORD(wParam) == 141) {
			switch (HIWORD(wParam)) {
			case LBN_DBLCLK: {
				SendDlgItemMessage(hwnd, 146, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
				SendDlgItemMessage(hwnd,147, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);

				int indice = 0;

				indice = SendDlgItemMessage(hwnd, 141, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, 141, LB_GETTEXT, indice, (LPARAM)textoList);

				Promocion_aux = Promocion_inicio;
				while (Promocion_aux != nullptr && strcmp(textoList, Promocion_aux->Promnombre) != 0) {
					Promocion_aux = Promocion_aux->Promocion_sig;
				}
				if (Promocion_aux == nullptr) {

				}
				else
				{
					SetDlgItemText(hwnd, 142, Promocion_aux->Promnombre);
					SetDlgItemText(hwnd, 145, Promocion_aux->Promfecha);
					SetDlgItemText(hwnd, 143, Promocion_aux->Prommonto);
					SetDlgItemText(hwnd, 144, Promocion_aux->Promdescuento);

					if (strcmp(Promocion_aux->Promestatus, "Activa") == 0)
					{
						SendDlgItemMessage(hwnd, 146, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					}
					if (strcmp(Promocion_aux->Promestatus, "Inactiva") == 0)
					{
						SendDlgItemMessage(hwnd,147, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
					}
				}

			}break;
			}
		}

		if (LOWORD(wParam) == 152) {//Registrar
			int indice = 0;

			//Usuario* newInfoUser = new Usuario;
			Promocion_aux = Promocion_inicio;
			indice = SendDlgItemMessage(hwnd, 141, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, 141, LB_GETTEXT, indice, (LPARAM)textoList);


			//Modificar_Usuario(newInfoUser, textoList);


			while (Promocion_aux != nullptr && strcmp(Promocion_aux->Promnombre, textoList) != 0) {

				Promocion_aux = Promocion_aux->Promocion_sig;

			}
			if (Promocion_aux == nullptr) {
				MessageBeep(MB_OK);
				MessageBox(hwnd, "No se encontro promoción", "ADVERTENCIA", MB_OK | MB_ICONERROR);

			}
			else
			{
				int Opcion = MessageBox(0, "¿Seguro que desea modificar la promoción?", "AVISO", MB_YESNO | MB_ICONQUESTION);
				if (Opcion == IDYES) {
					if (IsDlgButtonChecked(hwnd, 146) == BST_CHECKED)
					{
						strcpy_s(Promocion_aux->Promestatus, "Activa");
					}
					if (IsDlgButtonChecked(hwnd,147) == BST_CHECKED)
					{
						strcpy_s(Promocion_aux->Promestatus, "Inactiva");
					}
					MessageBox(0, "Promoción modificado", "AVISO", MB_OK | MB_ICONINFORMATION);
					EndDialog(hwnd, 0);
					Escribir_Binario_Promocion();
					DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG14), hwnd, listapromo);
				}

			}




		}

		if (LOWORD(wParam) == 151) {//Eliminar
			int indice = 0;

			indice = SendDlgItemMessage(hwnd, 141, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, 141, LB_GETTEXT, indice, (LPARAM)textoList);

			int Opcion = MessageBox(0, "¿Seguro que desea eliminar la promoción?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			if (Opcion == IDYES) {
				Eliminar_Promocion(textoList);

				SendMessage(GetDlgItem(hwnd, 141), LB_DELETESTRING, (WPARAM)indice, 0);
				SendMessage(GetDlgItem(hPrinGlob, 141), LB_RESETCONTENT, 0, 0);
				Escribir_Binario_Promocion();

				Promocion_aux = Promocion_inicio;
				while (Promocion_aux != nullptr) {
					SendMessage(GetDlgItem(hPrinGlob, 141), LB_ADDSTRING, 0, (LPARAM)Promocion_aux->Promnombre);
					Promocion_aux = Promocion_aux->Promocion_sig;
				}
			}

		}


		if (LOWORD(wParam) == 150) {//modificar
			if (strcmp(NUsuario->usuariotipo, "Administrador") == 0)
			{
				EnableWindow(GetDlgItem(hwnd, 146), true);
				EnableWindow(GetDlgItem(hwnd, 147), true);
			}
		}

		if (LOWORD(wParam) == 153) {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_DIALOG3), hwnd, PantallaPrincipal);
		}
		//switch (LOWORD(wParam)) {
		//case 1101: {//Regresar a menu
		//	EndDialog(hwnd, 0);
		//}break;
		//}
	}break;
	}
	return false;
}

void Agregar_usuario(Usuario* nuevo_u) 
{
	if (Usuario_inicio == nullptr) {
		Usuario_inicio = new Usuario;
		Usuario_aux = Usuario_inicio;

		Usuario_aux->Usuario_sig = nullptr;
		Usuario_aux->Usuario_ant = nullptr;

		strcpy_s(Usuario_aux->nombreus, nuevo_u->nombreus);
		strcpy_s(Usuario_aux->nombrecompleto, nuevo_u->nombrecompleto);
		strcpy_s(Usuario_aux->fotous, nuevo_u->fotous);
		strcpy_s(Usuario_aux->contraus, nuevo_u->contraus);
		strcpy_s(Usuario_aux->comercious, nuevo_u->comercious);
		strcpy_s(Usuario_aux->fotous, nuevo_u->fotous);
		strcpy_s(Usuario_aux->direccionus, nuevo_u->direccionus);
		strcpy_s(Usuario_aux->primera, nuevo_u->primera);
		strcpy_s(Usuario_aux->usuariotipo, nuevo_u->usuariotipo);
		/* Usuario_aux->Usuario_tipo = nuevo_u->Usuario_tipo;*/
	}
	else {
		Usuario_aux = Usuario_inicio;

		while (Usuario_aux->Usuario_sig != nullptr)
		{
			Usuario_aux = Usuario_aux->Usuario_sig;
		}

		Usuario_aux->Usuario_sig = new Usuario;

		Usuario_aux->Usuario_sig->Usuario_sig = nullptr;
		Usuario_aux->Usuario_sig->Usuario_ant = Usuario_aux;

		Usuario_aux = Usuario_aux->Usuario_sig;

		strcpy_s(Usuario_aux->nombreus, nuevo_u->nombreus);
		strcpy_s(Usuario_aux->contraus, nuevo_u->contraus);
		strcpy_s(Usuario_aux->nombrecompleto, nuevo_u->nombrecompleto);
		strcpy_s(Usuario_aux->comercious, nuevo_u->comercious);
		strcpy_s(Usuario_aux->fotous, nuevo_u->fotous);
		/*Usuario_aux->Usuario_tipo = nuevo_u->Usuario_tipo;*/
		strcpy_s(Usuario_aux->direccionus, nuevo_u->direccionus);
		strcpy_s(Usuario_aux->usuariotipo, nuevo_u->usuariotipo);
		strcpy_s(Usuario_aux->primera, nuevo_u->primera);

	}
	MessageBox(0, "Usuario registrado", "AVISO", MB_OK);
}
void Escribir_Binario_Usuario()
{
	Usuario_aux = Usuario_inicio;

	ofstream escribirUsuario;
	escribirUsuario.open("C:\\Users\\Shado\\OneDrive\\Documentos\\codigos\\avance3\\avance3\\Usuario.bin", ios::out | ios::binary | ios::trunc); 

	if (escribirUsuario.is_open()) {
		while (Usuario_aux != nullptr)
		{
			escribirUsuario.write((char*)Usuario_aux, sizeof(Usuario));
			Usuario_aux = Usuario_aux->Usuario_sig;
		}

		escribirUsuario.close();
	}
	else
	{
		system("PAUSE");
	}
} 
void Leer_Binario_Usuario()
{
	ifstream archivoUsuario;
	archivoUsuario.open("C:\\Users\\Shado\\OneDrive\\Documentos\\codigos\\avance3\\avance3\\Usuario.bin", ios::in | ios::binary );


	if (archivoUsuario.is_open()) {

		Usuario* Usuario_leido = new Usuario;

		while (!archivoUsuario.read((char*)Usuario_leido, sizeof(Usuario)).eof()) {

			if (Usuario_inicio == nullptr) {
				Usuario_inicio = Usuario_leido;
				Usuario_inicio->Usuario_sig = nullptr;
				Usuario_inicio->Usuario_ant = nullptr;
				Usuario_aux = Usuario_inicio;
			}
			else {
				Usuario_aux->Usuario_sig = Usuario_leido;
				Usuario_aux->Usuario_sig->Usuario_ant = Usuario_aux;
				Usuario_aux = Usuario_aux->Usuario_sig;
				Usuario_aux->Usuario_sig = nullptr;
				//fin = aux;
			}

			Usuario_leido = new Usuario;
		}
		archivoUsuario.close();
		delete Usuario_leido;
	}
	else {
		system("PAUSE");
	}
}
void Eliminar_Usuario(char nomUsuario[50]) {
	Usuario_aux = Usuario_inicio;

	if (Usuario_aux == nullptr)
	{
		MessageBox(0, "No hay Usuario registrados", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		while (Usuario_aux != nullptr && strcmp(Usuario_aux->nombreus, nomUsuario) != 0) {

			Usuario_aux = Usuario_aux->Usuario_sig;
		}
		if (Usuario_aux == nullptr) {
			MessageBox(0, "Usuario no encontrado", "AVISO", MB_OK);
		}
		else if (Usuario_aux == Usuario_inicio) {	//El nodo es el primero 

			if (Usuario_aux->Usuario_sig == nullptr) {

				Usuario_inicio = nullptr;
				delete Usuario_aux;
				Usuario_aux = Usuario_inicio;
			}
			else {
				Usuario_inicio = Usuario_inicio->Usuario_sig;
				Usuario_aux->Usuario_sig->Usuario_ant = nullptr;
				delete Usuario_aux;
				Usuario_aux = Usuario_inicio;

			}

			MessageBox(0, "Usuario eliminado", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
		else {

			if (Usuario_aux->Usuario_sig == nullptr) { //Si el nodo es el último
				Usuario_aux->Usuario_ant->Usuario_sig = nullptr;
				delete Usuario_aux;
				Usuario_aux = Usuario_inicio;
			}
			else {
				Usuario_aux->Usuario_sig->Usuario_ant = Usuario_aux->Usuario_ant;
				Usuario_aux->Usuario_ant->Usuario_sig = Usuario_aux->Usuario_sig;
				delete Usuario_aux;
				Usuario_aux = Usuario_inicio;
			}

			MessageBox(0, "Usuario eliminado", "AVISO", MB_OK | MB_ICONINFORMATION);

		}
	}

}


void Agregar_Cliente(Cliente* nuevo) {


	if (Cliente_inicio == nullptr) {
		Cliente_inicio = new Cliente;
		Cliente_aux = Cliente_inicio;

		Cliente_aux->Cliente_sig = nullptr;
		Cliente_aux->Cliente_ant = nullptr;

		strcpy_s(Cliente_aux->nombrecliente, nuevo->nombrecliente);
		strcpy_s(Cliente_aux->clienteemail, nuevo->clienteemail);
		strcpy_s(Cliente_aux->comerciocliente, nuevo->comerciocliente);
		strcpy_s(Cliente_aux->fechacliente, nuevo->fechacliente);

	}
	else {
		Cliente_aux = Cliente_inicio;
		while (Cliente_aux->Cliente_sig != nullptr)
		{
			Cliente_aux = Cliente_aux->Cliente_sig;
		}

		Cliente_aux->Cliente_sig = new Cliente;

		Cliente_aux->Cliente_sig->Cliente_sig = nullptr;
		Cliente_aux->Cliente_sig->Cliente_ant = Cliente_aux;

		Cliente_aux = Cliente_aux->Cliente_sig;

		strcpy_s(Cliente_aux->nombrecliente, nuevo->nombrecliente);
		strcpy_s(Cliente_aux->clienteemail, nuevo->clienteemail);
		strcpy_s(Cliente_aux->comerciocliente, nuevo->comerciocliente);
		strcpy_s(Cliente_aux->fechacliente, nuevo->fechacliente);
		Cliente_aux->Cliente_sig = nullptr;
	}
	MessageBox(0, "Cliente registrado", "AVISO", MB_OK);
}
void Escribir_Binario_Cliente()
{
	Cliente_aux = Cliente_inicio;

	ofstream escribirCliente;
	escribirCliente.open("C:\\Users\\Shado\\OneDrive\\Documentos\\codigos\\avance3\\avance3\\Clientes.bin", ios::out | ios::binary | ios::trunc);
	if (escribirCliente.is_open()) {
		while (Cliente_aux != nullptr)
		{
			escribirCliente.write((char*)Cliente_aux, sizeof(Cliente));
			Cliente_aux = Cliente_aux->Cliente_sig;
		}

		escribirCliente.close();
	}
	else
	{
		system("PAUSE");
	}
}
void Leer_Binario_Cliente()
{
	ifstream archivoCliente;
	archivoCliente.open("C:\\Users\\Shado\\OneDrive\\Documentos\\codigos\\avance3\\avance3\\Clientes.bin", ios::in | ios::binary);

	if (archivoCliente.is_open()) {

		Cliente* Cliente_leido = new Cliente;

		while (!archivoCliente.read((char*)Cliente_leido, sizeof(Cliente)).eof()) {

			if (Cliente_inicio == nullptr) {
				Cliente_inicio = Cliente_leido;
				Cliente_inicio->Cliente_sig = nullptr;
				Cliente_inicio->Cliente_ant = nullptr;
				Cliente_aux = Cliente_inicio;
			}
			else {
				Cliente_aux->Cliente_sig = Cliente_leido;
				Cliente_aux->Cliente_sig->Cliente_ant = Cliente_aux;
				Cliente_aux = Cliente_aux->Cliente_sig;
				Cliente_aux->Cliente_sig = nullptr;
				//fin = aux;
			}

			Cliente_leido = new Cliente;
		}
		archivoCliente.close();
		delete Cliente_leido;
	}
	else {
		system("PAUSE");
	}
}
void Eliminar_Cliente(char nomCliente[50]) {
	Cliente_aux = Cliente_inicio;

	if (Cliente_aux == nullptr)
	{
		MessageBox(0, "No hay clientes registrados", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		while (Cliente_aux != nullptr && strcmp(Cliente_aux->nombrecliente, nomCliente) != 0) {

			Cliente_aux = Cliente_aux->Cliente_sig;
		}
		if (Cliente_aux == nullptr) {
			MessageBox(0, "Cliente no encontrado", "AVISO", MB_OK);
		}
		else if (Cliente_aux == Cliente_inicio) {	//El nodo es el primero 

			if (Cliente_aux->Cliente_sig == nullptr) {

				Cliente_inicio = nullptr;
				delete Cliente_aux;
				Cliente_aux = Cliente_inicio;
			}
			else {
				Cliente_inicio = Cliente_inicio->Cliente_sig;
				Cliente_aux->Cliente_sig->Cliente_ant = nullptr;
				delete Cliente_aux;
				Cliente_aux = Cliente_inicio;

			}

			MessageBox(0, "Cliente eliminado", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
		else {

			if (Cliente_aux->Cliente_sig == nullptr) { //Si el nodo es el último
				Cliente_aux->Cliente_ant->Cliente_sig = nullptr;
				delete Cliente_aux;
				Cliente_aux = Cliente_inicio;
			}
			else {
				Cliente_aux->Cliente_sig->Cliente_ant = Cliente_aux->Cliente_ant;
				Cliente_aux->Cliente_ant->Cliente_sig = Cliente_aux->Cliente_sig;
				delete Cliente_aux;
				Cliente_aux = Cliente_inicio;
			}

			MessageBox(0, "Cliente eliminado", "AVISO", MB_OK | MB_ICONINFORMATION);

		}
	}

}


void Agregar_Consumo(Consumo* nuevo) {


	if (Consumo_inicio == nullptr) {
		Consumo_inicio = new Consumo;
		Consumo_aux = Consumo_inicio;

		Consumo_aux->Consumo_sig = nullptr;
		Consumo_aux->Consumo_ant = nullptr;

		strcpy_s(Consumo_aux->Concliente, nuevo->Concliente);
		strcpy_s(Consumo_aux->Confecha, nuevo->Confecha);
		strcpy_s(Consumo_aux->Conmonto, nuevo->Conmonto);
		strcpy_s(Consumo_aux->Condescuento, nuevo-> Condescuento);
		strcpy_s(Consumo_aux->Consubtotal, nuevo->Consubtotal);
		strcpy_s(Consumo_aux->Contotal, nuevo->Contotal);
		strcpy_s(Consumo_aux->Concomercio, nuevo->Concomercio);
		strcpy_s(Consumo_aux->ConID, nuevo->ConID);
	}
	else {
		Consumo_aux = Consumo_inicio;

		while (Consumo_aux->Consumo_sig != nullptr)
		{
			Consumo_aux = Consumo_aux->Consumo_sig;
		}

		Consumo_aux->Consumo_sig = new Consumo;

		Consumo_aux->Consumo_sig->Consumo_sig = nullptr;
		Consumo_aux->Consumo_sig->Consumo_ant = Consumo_aux;

		Consumo_aux = Consumo_aux->Consumo_sig;

		strcpy_s(Consumo_aux->Concliente, nuevo->Concliente);
		strcpy_s(Consumo_aux->Conmonto, nuevo->Conmonto);
		strcpy_s(Consumo_aux->Confecha, nuevo->Confecha);
		strcpy_s(Consumo_aux->Condescuento, nuevo->Condescuento);
		strcpy_s(Consumo_aux->Consubtotal, nuevo->Consubtotal);
		strcpy_s(Consumo_aux->Contotal, nuevo->Contotal);
		strcpy_s(Consumo_aux->Concomercio, nuevo->Concomercio);
		strcpy_s(Consumo_aux->ConID, nuevo->ConID);
	}
	MessageBox(0, "Consumo registrado", "AVISO", MB_OK);
}
void Escribir_Binario_Consumo()
{
	Consumo_aux = Consumo_inicio;

	ofstream escribirConsumo;
	escribirConsumo.open("C:\\Users\\Shado\\OneDrive\\Documentos\\codigos\\avance3\\avance3\\Consumo.bin", ios::out | ios::binary | ios::trunc);

	if (escribirConsumo.is_open()) {
		while (Consumo_aux != nullptr)
		{
			escribirConsumo.write((char*)Consumo_aux, sizeof(Consumo));
			Consumo_aux = Consumo_aux->Consumo_sig;
		}

		escribirConsumo.close();
	}
	else
	{
		system("PAUSE");
	}
}
void Leer_Binario_Consumo()
{
	ifstream archivoConsumo;
	archivoConsumo.open("C:\\Users\\Shado\\OneDrive\\Documentos\\codigos\\avance3\\avance3\\Consumo.bin", ios::in | ios::binary);


	if (archivoConsumo.is_open()) {

		Consumo* Consumo_leido = new Consumo;

		while (!archivoConsumo.read((char*)Consumo_leido, sizeof(Consumo)).eof()) {

			if (Consumo_inicio == nullptr) {
				Consumo_inicio = Consumo_leido;
				Consumo_inicio->Consumo_sig = nullptr;
				Consumo_inicio->Consumo_ant = nullptr;
				Consumo_aux = Consumo_inicio;
			}
			else {
				Consumo_aux->Consumo_sig = Consumo_leido;
				Consumo_aux->Consumo_sig->Consumo_ant = Consumo_aux;
				Consumo_aux = Consumo_aux->Consumo_sig;
				Consumo_aux->Consumo_sig = nullptr;
				//fin = aux;
			}

			Consumo_leido = new Consumo;
		}
		archivoConsumo.close();
		delete Consumo_leido;
	}
	else {
		system("PAUSE");
	}
}

void Agregar_Promocion(Promocion* nuevo) {


	if (Promocion_inicio == nullptr) {
		Promocion_inicio = new Promocion;
		Promocion_aux = Promocion_inicio;

		Promocion_aux->Promocion_sig = nullptr;
		Promocion_aux->Promocion_ant = nullptr;

		strcpy_s(Promocion_aux->Promnombre, nuevo->Promnombre);
		strcpy_s(Promocion_aux->Prommonto, nuevo->Prommonto);
		strcpy_s(Promocion_aux->Promdescuento, nuevo->Promdescuento);
		strcpy_s(Promocion_aux->Promcomercio, nuevo->Promcomercio);
		strcpy_s(Promocion_aux->Promestatus, nuevo->Promestatus);
		strcpy_s(Promocion_aux->Promfecha, nuevo->Promfecha);
		/*strcpy_s(Promocion_aux->Promestatus, nuevo->Promestatus);*/

	/*	Promocion_aux->Prommonto = nuevo->Prommonto;
		Promocion_aux->Promdescuento = nuevo->Promdescuento;*/

	}
	else {
		Promocion_aux = Promocion_inicio;
		while (Promocion_aux->Promocion_sig != nullptr)
		{
			Promocion_aux = Promocion_aux->Promocion_sig;
		}

		Promocion_aux->Promocion_sig = new Promocion;

		Promocion_aux->Promocion_sig->Promocion_sig = nullptr;
		Promocion_aux->Promocion_sig->Promocion_ant = Promocion_aux;

		Promocion_aux = Promocion_aux->Promocion_sig;

		strcpy_s(Promocion_aux->Promnombre, nuevo->Promnombre);
		strcpy_s(Promocion_aux->Prommonto, nuevo->Prommonto);
		strcpy_s(Promocion_aux->Promdescuento, nuevo->Promdescuento);
		strcpy_s(Promocion_aux->Promcomercio, nuevo->Promcomercio);
		strcpy_s(Promocion_aux->Promestatus, nuevo->Promestatus);
		strcpy_s(Promocion_aux->Promfecha, nuevo->Promfecha);
		/*strcpy_s(Promocion_aux->Promestatus, nuevo->Promestatus);*/

		/*Promocion_aux->Prommonto = nuevo->Prommonto;
		Promocion_aux->Promdescuento = nuevo->Promdescuento;*/
		Promocion_aux->Promocion_sig = nullptr;
	}
	MessageBox(0, "Promocion registrada", "AVISO", MB_OK);
}
void Escribir_Binario_Promocion()
{
	Promocion_aux = Promocion_inicio;

	ofstream escribirPromocion;
	escribirPromocion.open("C:\\Users\\Shado\\OneDrive\\Documentos\\codigos\\avance3\\avance3\\Promocion.bin", ios::out | ios::binary | ios::trunc);

	if (escribirPromocion.is_open()) {
		while (Promocion_aux != nullptr)
		{
			escribirPromocion.write((char*)Promocion_aux, sizeof(Promocion));
			Promocion_aux = Promocion_aux->Promocion_sig;
		}

		escribirPromocion.close();
	}
	else
	{
		system("PAUSE");
	}
}
void Leer_Binario_Promocion()
{
	ifstream archivoPromocion;
	archivoPromocion.open("C:\\Users\\Shado\\OneDrive\\Documentos\\codigos\\avance3\\avance3\\Promocion.bin", ios::in | ios::binary);


	if (archivoPromocion.is_open()) {

		Promocion* Promocion_leido = new Promocion;

		while (!archivoPromocion.read((char*)Promocion_leido, sizeof(Promocion)).eof()) {

			if (Promocion_inicio == nullptr) {
				Promocion_inicio = Promocion_leido;
				Promocion_inicio->Promocion_sig = nullptr;
				Promocion_inicio->Promocion_ant = nullptr;
				Promocion_aux = Promocion_inicio;
			}
			else {
				Promocion_aux->Promocion_sig = Promocion_leido;
				Promocion_aux->Promocion_sig->Promocion_ant = Promocion_aux;
				Promocion_aux = Promocion_aux->Promocion_sig;
				Promocion_aux->Promocion_sig = nullptr;
				//fin = aux;
			}

			Promocion_leido = new Promocion;
		}
		archivoPromocion.close();
		delete Promocion_leido;
	}
	else {
		system("PAUSE");
	}
}
void Eliminar_Promocion(char nomPromocion[50]) {
	Promocion_aux = Promocion_inicio;

	if (Promocion_aux == nullptr)
	{
		MessageBox(0, "No hay promociones registradas", "AVISO", MB_OK | MB_ICONINFORMATION);
	}
	else {
		while (Promocion_aux != nullptr && strcmp(Promocion_aux->Promnombre, nomPromocion) != 0) {

			Promocion_aux = Promocion_aux->Promocion_sig;
		}
		if (Promocion_aux == nullptr) {
			MessageBox(0, "Promocion no encontrada", "AVISO", MB_OK);
		}
		else if (Promocion_aux == Promocion_inicio) {	//El nodo es el primero 

			if (Promocion_aux->Promocion_sig == nullptr) {

				Promocion_inicio = nullptr;
				delete Promocion_aux;
				Promocion_aux = Promocion_inicio;
			}
			else {
				Promocion_inicio = Promocion_inicio->Promocion_sig;
				Promocion_aux->Promocion_sig->Promocion_ant = nullptr;
				delete Promocion_aux;
				Promocion_aux = Promocion_inicio;

			}

			MessageBox(0, "Promocion eliminada", "AVISO", MB_OK | MB_ICONINFORMATION);
		}
		else {

			if (Promocion_aux->Promocion_sig == nullptr) { //Si el nodo es el último
				Promocion_aux->Promocion_ant->Promocion_sig = nullptr;
				delete Promocion_aux;
				Promocion_aux = Promocion_inicio;
			}
			else {
				Promocion_aux->Promocion_sig->Promocion_ant = Promocion_aux->Promocion_ant;
				Promocion_aux->Promocion_ant->Promocion_sig = Promocion_aux->Promocion_sig;
				delete Promocion_aux;
				Promocion_aux = Promocion_inicio;
			}

			MessageBox(0, "Promoción eliminada", "AVISO", MB_OK | MB_ICONINFORMATION);

		}
	}

}

