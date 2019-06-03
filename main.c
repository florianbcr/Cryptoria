#define ENOMEM 12

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <gmp.h>
#include <assert.h>
#include <sodium.h>
#include <openssl/sha.h>
#include <glib.h>
#include <gtk/gtk.h>

#include "utils/cmath.c"
#include "utils/ccrypto.c"
#include "utils/cutils.c"
#include "utils/crsa.c"

#define SELECT1 "1024 bits (Normal)"
#define SELECT2 "2048 bits (Recommanded)"
#define SELECT3 "4096 bits (Military)"
#define TAB1 "New Key"
#define TAB2 "Encryption"
#define TAB3 "Decryption"

void openWindow();
void generateKey(GtkWidget *pBtn, gpointer data[]);
void encrypt_signal(GtkWidget *pBtn, gpointer data[]);
void decrypt_signal(GtkWidget *pBtn, gpointer data[]);

int main(int argc, char **argv) {
	//Innitialise la librairie sodium
	if (sodium_init() < 0) {
        	return 1; 
    }
	//On innitialise la librairie gtk
	gtk_init(&argc,&argv);
	/*
	RSA rsa = newkey(1024);
	char msg[] = "coucou";
	char msg_encoded[10000] = "";
	char msg_decoded[10000] = "";
	encode(rsa, msg, msg_encoded);
	decode(rsa, msg_encoded, msg_decoded);
	printf("msg: %s\n", msg);
	printf("msg_encoded: %s\n", msg_encoded);
	printf("msg_decoded: %s\n", msg_decoded);
	*/
	openWindow();
	
	return EXIT_SUCCESS;
}

void openWindow() {
	//On crée la fenetre principal
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "Cryptoria");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 700);
    gtk_widget_set_size_request(GTK_WIDGET(window), 600, 700);
	// On ajoute un espace de 5 sur les bords de la fenetre
    gtk_container_set_border_width(GTK_CONTAINER(window), 5);
	//On associe la bouton X a la fermeture de la fenetre
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	
	//Creation de la Vbox principal
	GtkWidget *main_vbox = gtk_vbox_new(FALSE, 0);
    gtk_container_add(GTK_CONTAINER(window), main_vbox);
    /* Creation du GtkNotebook */
    GtkWidget *main_notebook = gtk_notebook_new();
    gtk_box_pack_start(GTK_BOX(main_vbox), main_notebook, TRUE, TRUE, 0);
	
	//On crée les titres des tab du notebook
	GtkWidget *tab1 = gtk_label_new(TAB1);
	GtkWidget *tab2 = gtk_label_new(TAB2);	
	GtkWidget *tab3 = gtk_label_new(TAB3);
	
	/*
	*	Ajout de la premiere page
	*/
	/* Création de la GtkBox verticale principal */
    GtkWidget *vbox_tab1 = gtk_vbox_new(FALSE, 0);
	/* Création du frame */
	GtkWidget *frame = gtk_frame_new("Key Size");
    gtk_container_border_width(GTK_CONTAINER(frame), 5);
    gtk_box_pack_start(GTK_BOX(vbox_tab1), frame, FALSE, FALSE, 0);
				
	GtkWidget *vboxFrame_tab1 = gtk_vbox_new(FALSE, 0);
   	gtk_container_add(GTK_CONTAINER(frame), vboxFrame_tab1);
				
    GtkWidget *button = gtk_radio_button_new_with_label(NULL, SELECT1);
   	gtk_box_pack_start (GTK_BOX(vboxFrame_tab1), button, TRUE, TRUE, 0);
	button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON (button), SELECT2);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON (button), TRUE);
    gtk_box_pack_start(GTK_BOX(vboxFrame_tab1), button, TRUE, TRUE, 0);
    button = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON (button), SELECT3);
    gtk_box_pack_start(GTK_BOX(vboxFrame_tab1), button, TRUE, TRUE, 0);
				
	//Bouton pour generé la clé
	GtkWidget *generate = gtk_button_new_with_label("Generate");
    gtk_box_pack_start(GTK_BOX(vbox_tab1), generate, FALSE, FALSE, 0);
    gtk_container_border_width(GTK_CONTAINER(generate), 5);
				
	//Creation du notebook pour afficher les clés
	GtkWidget *notebook = gtk_notebook_new();
    gtk_container_border_width(GTK_CONTAINER(notebook), 5);
	
	//Create de la page public key
	GtkWidget *notebook_publickey = gtk_label_new("Public Key");
	GtkWidget *scrollbar_publickey = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *vbox_scrollable_public_key = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(vbox_scrollable_public_key), scrollbar_publickey);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar_publickey), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
	GtkWidget *tv_publickey = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(tv_publickey), FALSE);
	//Changement du texte de la textview
	GtkTextBuffer *buffer_publickey = gtk_text_view_get_buffer(GTK_TEXT_VIEW (tv_publickey));
  	gtk_text_buffer_set_text (buffer_publickey, "No key generated...", -1);
	gtk_container_add(GTK_CONTAINER(scrollbar_publickey), tv_publickey);
	
	//Creation de la page private key
	GtkWidget *notebook_privatekey = gtk_label_new("Private Key");
	GtkWidget *scrollbar_privatekey = gtk_scrolled_window_new(NULL, NULL);
	GtkWidget *vbox_scrollable_private_key = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(vbox_scrollable_private_key), scrollbar_privatekey);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar_privatekey), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
	GtkWidget *tv_privatekey = gtk_text_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(tv_privatekey), FALSE);
	//Changement du texte de la textview
	GtkTextBuffer *buffer_privatekey = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tv_privatekey));
  	gtk_text_buffer_set_text (buffer_privatekey, "No key generated...", -1);
	gtk_container_add(GTK_CONTAINER(scrollbar_privatekey), tv_privatekey);
	
	//On ajoute les pages aux notebook
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox_scrollable_public_key, notebook_publickey);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox_scrollable_private_key, notebook_privatekey);
    gtk_box_pack_start(GTK_BOX(vbox_tab1), notebook, TRUE, TRUE, 0);
				
	//On associe le signal au bouton de generation
	gpointer tab[3] = {button, tv_publickey, tv_privatekey};
	g_signal_connect(G_OBJECT(generate), "clicked", G_CALLBACK(generateKey), tab);
	
	/*
	*	Ajout de la deuxieme page page
	*/
	/* Création de la GtkBox verticale principal */
    GtkWidget *vbox_tab2 = gtk_vbox_new(FALSE, 0);
	/* Création du frame pour la clé public */
	GtkWidget *frame_publickey = gtk_frame_new("Public key");
    gtk_container_border_width(GTK_CONTAINER(frame_publickey), 5);
    gtk_box_pack_start(GTK_BOX(vbox_tab2), frame_publickey, TRUE, TRUE, 0);
	/* Création du frame pour le message */
	GtkWidget *frame_encryption = gtk_frame_new("Your message");
    gtk_container_border_width(GTK_CONTAINER(frame_encryption), 5);
    gtk_box_pack_start(GTK_BOX(vbox_tab2), frame_encryption, TRUE, TRUE, 0);
	
	
	GtkWidget *scrollbar1 = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar1), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
	GtkWidget *scrollbar2 = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar2), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
	
	GtkWidget *vboxFrame_key_tab2 = gtk_vbox_new(FALSE, 0);
   	gtk_container_add(GTK_CONTAINER(frame_publickey), vboxFrame_key_tab2);
    GtkWidget *tv_key_public = gtk_text_view_new();
   	gtk_box_pack_start (GTK_BOX(vboxFrame_key_tab2), scrollbar1, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(scrollbar1), tv_key_public);
	
	GtkWidget *vboxFrame_message_tab2 = gtk_vbox_new(FALSE, 0);
   	gtk_container_add(GTK_CONTAINER(frame_encryption), vboxFrame_message_tab2);
    GtkWidget *tv_plaintext = gtk_text_view_new();
   	gtk_box_pack_start (GTK_BOX(vboxFrame_message_tab2), scrollbar2, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(scrollbar2), tv_plaintext);
	
	//Bouton pour chiffré le message
	GtkWidget *encrypt = gtk_button_new_with_label(TAB2);
    gtk_box_pack_start(GTK_BOX(vbox_tab2), encrypt, FALSE, FALSE, 0);
    gtk_container_border_width(GTK_CONTAINER(encrypt), 5);
	
	gpointer tab_encrypt[2] = {tv_key_public, tv_plaintext};
	g_signal_connect(G_OBJECT(encrypt), "clicked", G_CALLBACK(encrypt_signal), tab_encrypt);
	
	/*
	*	Ajout de la troisieme page page
	*/
	/* Création de la GtkBox verticale principal */
    GtkWidget *vbox_tab3 = gtk_vbox_new(FALSE, 0);
	/* Création du frame pour la clé public */
	GtkWidget *frame_privatekey = gtk_frame_new("Private key");
    gtk_container_border_width(GTK_CONTAINER(frame_privatekey), 5);
    gtk_box_pack_start(GTK_BOX(vbox_tab3), frame_privatekey, TRUE, TRUE, 0);
	/* Création du frame pour le message */
	GtkWidget *frame_decryption = gtk_frame_new("Encrypted message");
    gtk_container_border_width(GTK_CONTAINER(frame_decryption), 5);
    gtk_box_pack_start(GTK_BOX(vbox_tab3), frame_decryption, TRUE, TRUE, 0);
	
	GtkWidget *scrollbar3 = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar3), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
	GtkWidget *scrollbar4 = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrollbar4), GTK_POLICY_ALWAYS, GTK_POLICY_ALWAYS);
	
	GtkWidget *vboxFrame_key_tab3 = gtk_vbox_new(FALSE, 0);
   	gtk_container_add(GTK_CONTAINER(frame_privatekey), vboxFrame_key_tab3);
    GtkWidget *tv_key_private = gtk_text_view_new();
   	gtk_box_pack_start (GTK_BOX(vboxFrame_key_tab3), scrollbar3, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(scrollbar3), tv_key_private);
	
	GtkWidget *vboxFrame_message_tab3 = gtk_vbox_new(FALSE, 0);
   	gtk_container_add(GTK_CONTAINER(frame_decryption), vboxFrame_message_tab3);
    GtkWidget *tv_encrypted = gtk_text_view_new();
   	gtk_box_pack_start (GTK_BOX(vboxFrame_message_tab3), scrollbar4, TRUE, TRUE, 0);
	gtk_container_add(GTK_CONTAINER(scrollbar4), tv_encrypted);
	
	//Bouton pour chiffré le message
	GtkWidget *decyption = gtk_button_new_with_label(TAB3);
    gtk_box_pack_start(GTK_BOX(vbox_tab3), decyption, FALSE, FALSE, 0);
    gtk_container_border_width(GTK_CONTAINER(decyption), 5);
	
	gpointer tab_decrypt[2] = {tv_key_private, tv_encrypted};
	g_signal_connect(G_OBJECT(decyption), "clicked", G_CALLBACK(decrypt_signal), tab_decrypt);
	
	//Ajout des pages au notebook
    gtk_notebook_append_page(GTK_NOTEBOOK(main_notebook), vbox_tab1, tab1);
    gtk_notebook_append_page(GTK_NOTEBOOK(main_notebook), vbox_tab2, tab2);
    gtk_notebook_append_page(GTK_NOTEBOOK(main_notebook), vbox_tab3, tab3);
	
	gtk_widget_show_all(window);
    gtk_main();
}

void generateKey(GtkWidget *pBtn, gpointer data[]) {
    const gchar *selected;
    /* Récupération de la liste des boutons */
    GSList *pList = gtk_radio_button_get_group(GTK_RADIO_BUTTON(data[0]));
    /* Parcours de la liste */
    while(pList) {
        /* Le bouton est-il sélectionné */
        if(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(pList->data))) {
            /* OUI -> on copie le label du bouton */
            selected = gtk_button_get_label(GTK_BUTTON(pList->data));
            /* On met la liste a NULL pour sortir de la boucle */
            pList = NULL;
        }
        else {
            /* NON -> on passe au bouton suivant */
            pList = g_slist_next(pList);
        }
    }
	int bits = 0;
	if(strcmp(selected, SELECT1) == 0) {
		bits = 1024;
	} else if(strcmp(selected, SELECT2) == 0) {
		bits = 2048;
	} else if(strcmp(selected, SELECT3) == 0) {
		bits = 4096;
	}
	
	RSA rsa = newkey(bits);
	char publicKey[5000], privateKey[5000];
	getPublicKey(rsa, publicKey);
	getPrivateKey(rsa, privateKey);
	
	GtkTextBuffer *buffer_publickey = gtk_text_view_get_buffer (GTK_TEXT_VIEW (data[1]));
  	gtk_text_buffer_set_text (buffer_publickey, publicKey, -1);
	
	GtkTextBuffer *buffer_privatekey = gtk_text_view_get_buffer (GTK_TEXT_VIEW(data[2]));
  	gtk_text_buffer_set_text(buffer_privatekey, privateKey, -1);
	
}

void encrypt_signal(GtkWidget *pBtn, gpointer data[]) {
	GtkTextIter start, end;
	GtkTextIter start1, end1;
	//On recupere la clé
	GtkTextBuffer *buffer_key = gtk_text_view_get_buffer(data[0]);
	gtk_text_buffer_get_bounds (buffer_key, &start, &end);
	char *key = gtk_text_buffer_get_text(buffer_key, &start, &end, FALSE);
	//On recupere le message texte
	GtkTextBuffer *buffer_msg = gtk_text_view_get_buffer(data[1]);
	gtk_text_buffer_get_bounds (buffer_msg, &start1, &end1);
	char *msg = gtk_text_buffer_get_text(buffer_msg, &start1, &end1, FALSE);
	
	RSA rsa = setRSAFromKey("", key);
	char msg_encrypted[10000] = "";
	char msg_encrypted_form[10000] = "";
	encode(rsa, msg, msg_encrypted);
	addFormMessage(msg_encrypted, msg_encrypted_form);
	
	GtkTextBuffer *buffer_privatekey = gtk_text_view_get_buffer(GTK_TEXT_VIEW (data[1]));
  	gtk_text_buffer_set_text (buffer_privatekey, msg_encrypted_form, -1);
}

void decrypt_signal(GtkWidget *pBtn, gpointer data[]) {
	GtkTextIter start, end;
	GtkTextIter start1, end1;
	//On recupere la clé
	GtkTextBuffer *buffer_key = gtk_text_view_get_buffer(data[0]);
	gtk_text_buffer_get_bounds (buffer_key, &start, &end);
	char *key = gtk_text_buffer_get_text(buffer_key, &start, &end, FALSE);
	//On recupere le message texte
	GtkTextBuffer *buffer_msg = gtk_text_view_get_buffer(data[1]);
	gtk_text_buffer_get_bounds (buffer_msg, &start1, &end1);
	char *msg_form = gtk_text_buffer_get_text(buffer_msg, &start1, &end1, FALSE);
	
	RSA rsa = setRSAFromKey(key, "");
	char msg_decrypted[10000] = "";
	char msg[10000] = "";
	removeFormMessage(msg_form, msg);
	decode(rsa, msg, msg_decrypted);
	
	GtkTextBuffer *buffer_privatekey = gtk_text_view_get_buffer(GTK_TEXT_VIEW (data[1]));
  	gtk_text_buffer_set_text (buffer_privatekey, msg_decrypted, -1);
}
