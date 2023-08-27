$tag = $scenario->getTags();
$courant = $tag['#courant#'];
$req = 'curl -i -GET "http://192.168.0.63:9200/\$SC%20'.$courant.'"';
$scenario->setLog('DEBUG REQUETTE : '.$req);
$output0 = shell_exec($req);
$scenario->setLog('DEBUG RETOUR : '.$output0);