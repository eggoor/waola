using WaolaCli;

namespace WaolaWPF.Models
{
	public interface IPermanentStorage
	{
		void Load(IScanner scanner);

		void Save(IScanner scanner);
	}
}
