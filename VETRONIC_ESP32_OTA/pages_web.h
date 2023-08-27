ACText(caption,"<h2>Communication avec la borne VETRONIC</h2>","text-align:center;color:#2f4f4f;margin:10px;","",AC_Tag_BR);
ACText(caption_results,"<h2>Réponse de la borne :</h2>","text-align:center;color:#2f4f4f;margin:10px;","",AC_Tag_BR);
ACText(content);
ACText(content2,"Sans <i>commande_Vetronic</i> spécifiée, le module interroge le status de la borne, les courants et la tension de charge en une seule requête.","","",AC_Tag_BR);
ACText(results,"","text-align:left","",AC_Tag_BR);
ACText(version);
ACInput(commande,"","Commande","","help",AC_Tag_None,AC_Input_Text);
ACSubmit(envoi, "Envoi", "/results");
ACSubmit(retour, "Retour", "/");
ACElement(newline, "<hr>");
ACText(caption_jeedom,"<h2>Configuration de Jeedom</h2>","text-align:center;color:#2f4f4f;margin:10px;","",AC_Tag_BR);
ACInput(ip_jeedom,"","Adresse IP de JEEDOM","^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$","",AC_Tag_BR);
ACInput(api_key_jeedom,"","Clé API","","",AC_Tag_BR);
ACInput(id_tension,"","ID Tension","","",AC_Tag_BR,AC_Input_Number);
ACInput(id_courant,"","ID Courant","","",AC_Tag_BR,AC_Input_Number);
ACInput(id_courant_max,"","ID Courant Max","","",AC_Tag_BR,AC_Input_Number);
ACInput(id_txt_status,"","ID Texte Status","","",AC_Tag_BR,AC_Input_Number);
ACInput(id_code_status,"","ID Code Status","","",AC_Tag_BR,AC_Input_Number);
ACInput(freq_update_evse,"","Fréquence de rafraichissement (ms)","","",AC_Tag_BR,AC_Input_Number);
ACText(caption_save_jeedom,"<h2>Paramètres mémorisés.</h2>","text-align:center;color:#2f4f4f;margin:10px;","",AC_Tag_BR);
ACSubmit(save, "Save", "/jeedom_save");

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Root Page
/////////////////////////////////////////////////////////////////////////////////////////////////////
AutoConnectAux page_root("/", "VETRONIC EVSE", false, {
  caption,
  content,
  content2,
  commande, 
  envoi,
  newline,
  version
});

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Root results
/////////////////////////////////////////////////////////////////////////////////////////////////////
AutoConnectAux page_results("/results", "VETRONIC EVSE", false, {
  caption_results,
  results,
  retour
});

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Page Jeedom
/////////////////////////////////////////////////////////////////////////////////////////////////////
AutoConnectAux page_jeedom("/jeedom", "Jeedom", true, {
  caption_jeedom,
  ip_jeedom,
  api_key_jeedom,
  id_code_status,
  id_txt_status,
  id_tension,
  id_courant,
  id_courant_max,
  freq_update_evse,
  save
});
/////////////////////////////////////////////////////////////////////////////////////////////////////
// Page Jeedom
/////////////////////////////////////////////////////////////////////////////////////////////////////
AutoConnectAux page_jeedom_save("/jeedom_save", "Jeedom", false, {
  caption_save_jeedom,
  results,
  retour
});
