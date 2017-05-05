%{
	Descobre valores para divisão
	(vs*mult >> desl == (u64) (vs/m))
	
	vs - Valores
	m  - Módulo
%}
function [mult desl] = mdl(vs, m)
	
	% Inteiro de 64 bits
	vs = uint64(vs);
	
	mult = uint64(0);
	certo = idivide(vs, m);
	
	% Quantidade de bits de max(vs)
	bits = 0;
	v = max(vs);
	while v
		bits++;
		v = bitshift(v, -1);
	end
	bits
	
	% Bits da expansão de 1/m
	r = bin(m);
	z = find(r == 1)(1) - 1; % Zeros iniciais
	mult = zeros(1, 64);
	mult(1:bits+1) = r(z+bits+1:-1:z+1);
	mult = bitpack(mult == 1, "uint64");
	
	% Arredonda para cima
	mult = mult+1;
	desl = z + bits + 1;
	
	%{
	fprintf("%d %d\n",
		all(bitshift(vs*(mult  ), -desl) == certo),
		all(bitshift(vs*(mult+1), -desl) == certo)
	);
	%}
	
	if ~all(bitshift(vs*mult, -desl) == certo)
		warning("Não deu certo!");
		mult = -1;
		desl = -1;
		return
	end
	
	%{
	% Verifica se tem 0 desnecessário no final
	% Só seria útil se bits == 64 (caso que requereria tratamento especial...)
	if ~(mult & 1)
		mult = bitshift(mult, -1);
		desl = desl - 1;
	end
	%}
	
	if (nargout < 2)
		mult = [mult desl];
	end
	
end
