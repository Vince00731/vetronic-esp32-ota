/////////////////////////////////////////////////////////////////////////////////////////////////////
// Root Page
/////////////////////////////////////////////////////////////////////////////////////////////////////
const char PAGE_ROOT[] PROGMEM = R"(
{ 
  "title": "VETRONIC EVSE", 
  "uri": "/", 
  "menu": false, 
  "element": [
    { 
      "name": "caption", 
      "type": "ACText", 
      "value": "<h2>Communication avec la borne VETRONIC</h2>",  
      "style": "text-align:center;color:#2f4f4f;margin:10px;", 
      "posterior": "br"
    },
    { 
      "name": "content", 
      "type": "ACText"  
    },
    { 
      "name": "content2", 
      "type": "ACText",  
      "value": "Sans <i>commande_Vetronic</i> spécifiée, le module interroge le status de la borne, les courants et la tension de charge en une seule requête.<br>"
    },
    { 
      "name": "commande", 
      "type": "ACInput", 
      "label": "Commande",
      "placeholder": "help",
      "global": true
    },
    {
      "name": "Envoi",
      "type": "ACSubmit",
      "value": "Envoi",
      "uri": "/results"
    },
    {
      "name": "newline",
      "type": "ACElement",
      "value": "<hr>"
    },
    { 
      "name": "version", 
      "type": "ACText"  
    }
  ]
}
)";

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Page results
/////////////////////////////////////////////////////////////////////////////////////////////////////
const char PAGE_RESULTS[] PROGMEM = R"(
{
  "uri": "/results",
  "title": "VETRONIC EVSE",
  "menu": false,
  "element": [
    { 
      "name": "caption", 
      "type": "ACText", 
      "value": "<h2>Réponse de la borne :</h2>",  
      "style": "text-align:left;color:#2f4f4f;margin:10px;" 
    },
    {
      "name": "results",
      "type": "ACText",
      "style": "text-align:left",
      "posterior": "br"
    },
    {
      "name": "Retour",
      "type": "ACSubmit",
      "value": "Retour",
      "uri": "/"
    }
  ]
}
)";