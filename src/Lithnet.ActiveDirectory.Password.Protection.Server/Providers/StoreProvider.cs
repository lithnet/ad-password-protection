using System;
using System.Collections.Generic;
using System.Text;

namespace Lithnet.ActiveDirectory.PasswordProtection.Server
{
    public class StoreProvider : IStoreProvider
    {
        private V3Store store;

        private readonly IRegistrySettingsProvider registryProvider;

        public StoreProvider(IRegistrySettingsProvider registryProvider)
        {
            this.registryProvider = registryProvider;
        }

        public IStore Store
        {
            get
            {
                if (store == null)
                {
                    store = new V3Store(this.registryProvider.StorePath);
                }

                return store;
            }
        }
    }
}
