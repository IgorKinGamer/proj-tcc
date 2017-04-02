% Busca o menor n�mero m (ou os buscar_N menores a partir de buscar_INI)
% tal que todos elementos de v m�dulo m t�m resultados diferentes

% Par�metros opcionais:
%     buscar_N   - Quantidade de n�meros a encontrar
%     buscar_INI - N�mero (m�dulo) a partir do qual buscar

function n = buscar(v, T)
	global buscar_N buscar_INI
	if isempty(buscar_N)
		buscar_N = 1;
	end
	if isempty(buscar_INI)
		buscar_INI = 1;
	end
	buscar_INI = max(buscar_INI, length(v));
	
	% Tempo limite
	if (nargin < 2) T = 3; end
	t0 = time;
	
	% �ltimo n�mero a se testar (todos al�m disso satisfazem)
	fim = max(v);
	% Pr�ximo n a ser testado
	nProx = buscar_INI;
	buscar_INI = [];
	i = 0;
	
	% N�o encontrou a quantidade desejada ainda
	while i < buscar_N
		n(++i) = nProx;
		% O n�mero n�o satisfaz
		while sum(calcMod(v, n(i))) != length(v)
			n(i)++;
			if (time - t0) > T
				n(i) = -1;
				return;
			end
		end
		% Se chegou aqui, algum satizfez
		% Limite
		if (n(i) == fim)
			return;
		end
		nProx = n(i)+1;
	end
	buscar_N = [];
end