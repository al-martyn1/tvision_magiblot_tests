

namespace tvision {

#if defined(TV_BARE_METAL)

void StdioCtl::write(const char *data, size_t bytes) const noexcept
{
}

void StdioCtl::read(char *data, size_t bytesToRead, size_t *pBytesReaded) const noexcept
{
}

#endif


} // namespace tvision